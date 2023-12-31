#include "nrf.h"
#include "nrf_log.h"
#include "nrf_timer.h"
#include "mem_manager.h"

#include <mbedtls/platform.h>
#include <openthread/heap.h>
#include <openthread/dataset.h>

#include "simple_thread.h"

static otInstance * m_ot_instance;

void __attribute__((weak)) thread_state_changed_callback(uint32_t flags, void * p_context)
{
    NRF_LOG_INFO("State changed! Flags: 0x%08lx Current role: %d\r\n",
                 flags, otThreadGetDeviceRole(p_context));

}

void thread_reset_active_timestamp() {
  otOperationalDataset dataset = {0};

  otDatasetGetActive(m_ot_instance, &dataset);
  dataset.mActiveTimestamp = 1;
  dataset.mComponents.mIsActiveTimestampPresent = 0;
  otError ret = otDatasetSetActive(m_ot_instance, &dataset);
  NRF_LOG_INFO("dataset commissioned %d, %d", otDatasetIsCommissioned(m_ot_instance), ret);

  // toggle thread enable to trigger searching for new networks
  otThreadSetEnabled(m_ot_instance, false);
  otThreadSetEnabled(m_ot_instance, true);

}

static void* ot_calloc(size_t n, size_t size)
{
    void *p_ptr = NULL;

    p_ptr = nrf_calloc(n, size);

    return p_ptr;
}

static void ot_free(void *p_ptr)
{
    nrf_free(p_ptr);
}

static void platform_init(void)
{
    APP_ERROR_CHECK(nrf_mem_init());

    int ret;

    ret = mbedtls_platform_set_calloc_free(ot_calloc, ot_free);
    ASSERT(ret == 0);

    ret = mbedtls_platform_setup(NULL);
    ASSERT(ret == 0);

    otHeapSetCAllocFree(ot_calloc, ot_free);
}


/**@brief Function for initializing the Thread Stack.
 */

void __attribute__((weak)) thread_init(const thread_config_t* config)
{
    otError error;
    static otOperationalDataset dataset = {0};

    otSysInit(0, NULL);

    platform_init();

    m_ot_instance = otInstanceInitSingle();
    ASSERT(m_ot_instance != NULL);


    NRF_LOG_INFO("Thread version: %s", otGetVersionString());
    NRF_LOG_INFO("Network name:   %s",
                 otThreadGetNetworkName(m_ot_instance));

    if (!otDatasetIsCommissioned(m_ot_instance) || config->autocommission)
    {
        if (config->ext_addr) {
          error = otLinkSetExtendedAddress(m_ot_instance, config->ext_addr);
          ASSERT(error == OT_ERROR_NONE);
        }

        // set up active dataset with channel, panid, masterkey
        if (config->channel) {
          dataset.mChannel = config->channel;
          dataset.mComponents.mIsChannelPresent = true;
        }
        if (config->panid) {
          dataset.mPanId = config->panid;
          dataset.mComponents.mIsPanIdPresent = true;
        }

        memcpy(&dataset.mMasterKey, config->masterkey, sizeof(dataset.mMasterKey));
        dataset.mComponents.mIsMasterKeyPresent = config->masterkey != NULL;

        // set active dataset
        otDatasetSetActive(m_ot_instance, &dataset);
    }

    error = otPlatRadioSetTransmitPower(m_ot_instance, config->tx_power);
    ASSERT(error == OT_ERROR_NONE);
    int8_t tx_power_set;
    otPlatRadioGetTransmitPower(m_ot_instance, &tx_power_set);
    NRF_LOG_INFO("TX Power: %d dBm", tx_power_set);


    otLinkModeConfig mode;
    memset(&mode, 0, sizeof(mode));

    if (config->sed) {
      // sleepy end device
      mode.mSecureDataRequests = true;
      mode.mRxOnWhenIdle       = false; // Join network as SED.
      otLinkSetPollPeriod(m_ot_instance, config->poll_period);
    }
    else {
      // regular device
      mode.mRxOnWhenIdle       = true;
      mode.mSecureDataRequests = true;
      mode.mDeviceType         = true;
      mode.mNetworkData        = true;
    }

    error = otThreadSetLinkMode(m_ot_instance, mode);
    ASSERT(error == OT_ERROR_NONE);

    if (config->child_period != 0) {
        otThreadSetChildTimeout(m_ot_instance, config->child_period);
    }

    error = otIp6SetEnabled(m_ot_instance, true);
    ASSERT(error == OT_ERROR_NONE);

    if (otDatasetIsCommissioned(m_ot_instance) || config->autocommission)
    {
      error = otThreadSetEnabled(m_ot_instance, true);
      ASSERT(error == OT_ERROR_NONE);

      NRF_LOG_INFO("Thread interface has been enabled.");
      NRF_LOG_INFO("802.15.4 Channel: %d", otLinkGetChannel(m_ot_instance));
      NRF_LOG_INFO("802.15.4 PAN ID:  0x%04x", otLinkGetPanId(m_ot_instance));
      NRF_LOG_INFO("rx-on-when-idle:  %s", otThreadGetLinkMode(m_ot_instance).mRxOnWhenIdle ?
          "enabled" : "disabled");
    }

    otSetStateChangedCallback(m_ot_instance, thread_state_changed_callback, m_ot_instance);
}

void fix_errata_78_in_nrf_802154(void)
{
    /* Temporary workaround for anomaly 78. When timer is stopped using STOP task
     * its power consumption will be higher.
     * Issuing task SHUTDOWN fixes this condition. This have to be removed when
     * proper fix will be pushed to nRF-IEEE-802.15.4-radio-driver.
     */
    if ((otPlatRadioGetState(m_ot_instance) == OT_RADIO_STATE_SLEEP) ||
        (otPlatRadioGetState(m_ot_instance) == OT_RADIO_STATE_DISABLED))
    {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_SHUTDOWN);
    }
    #if (__FPU_USED == 1)
    __set_FPSCR(__get_FPSCR() & ~(0x0000009F));
    (void) __get_FPSCR();
    NVIC_ClearPendingIRQ(FPU_IRQn);
    #endif
}

void __attribute__((weak)) thread_sleep(void)
{
    ASSERT(m_ot_instance != NULL);

    // Enter sleep state if no more tasks are pending.
    if (!otTaskletsArePending(m_ot_instance))
    {
        fix_errata_78_in_nrf_802154();

//#ifdef SOFTDEVICE_PRESENT
//        ret_code_t err_code = sd_app_evt_wait();
//        ASSERT(err_code == NRF_SUCCESS);
//#else
        __WFE();
//#endif
    }
}

void __attribute__((weak)) thread_process(void)
{
    ASSERT(m_ot_instance != NULL);
    otTaskletsProcess(m_ot_instance);
    otSysProcessDrivers(m_ot_instance);
}

otInstance * thread_get_instance(void) {
  return m_ot_instance;
}


