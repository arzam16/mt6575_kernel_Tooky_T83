
$(call codebase-path)

# project name
ifeq ($(findstring vee5,$(TARGET_PRODUCT)),vee5)
LGE_DWS      := _v5
else
LGE_DWS      := _v4
endif

# single/dual/triple SIM
ifeq ($(MTK_SHARE_MODEM_CURRENT),1)
LGE_DWS      := $(LGE_DWS)_1
else
  ifeq ($(MTK_SHARE_MODEM_CURRENT),2)
    LGE_DWS  := $(LGE_DWS)_2
  else
    LGE_DWS  := $(LGE_DWS)_3
  endif
endif

# NFC support
ifeq ($(MTK_NFC_SUPPORT),yes)
LGE_DWS      := $(LGE_DWS)_nfc
endif

# DOMEKEY support
ifeq ($(LGE_USE_DOME_KEY),yes)
LGE_DWS      := $(LGE_DWS)_dome
endif

# LGE_CHANGE_S 2012-11-06 hyun.yoo@lge.com : check REV Version
# REV A or B check
ifeq ($(HW_REVB), TRUE)
LGE_DWS     := $(LGE_DWS)_b
#PRODUCT_PROPERTY_OVERRIDES += \
# ro.revision = 0.B
else
  ifeq ($(HW_REVC), TRUE)
    LGE_DWS     := $(LGE_DWS)_c
  else
    ifeq ($(HW_REV1), TRUE)
      LGE_DWS     := $(LGE_DWS)_1
    else
      LGE_DWS     := $(LGE_DWS)_a
    endif
  endif
#PRODUCT_PROPERTY_OVERRIDES += \
# ro.revision = 0.A
endif
# LGE_CHANGE_E 2012-11-06 hyun.yoo@lge.com : check REV Version
# modify ProjectConfig.mk
ifeq ($(findstring vee5,$(TARGET_PRODUCT)),vee5)
BASE_PROJECT=lge75_v5_reva_jb
else
BASE_PROJECT=lge75_v4_jb
endif

LGE_PRJ_MF=$(MTK_ROOT_CONFIG)/$(BASE_PROJECT)/ProjectConfig.mk
LGE_PRJ_MF_TEMP=$(MTK_ROOT_CONFIG)/$(BASE_PROJECT)/ProjectConfig.temp


#define .lge.config.change
#$(shell sed /$(1)/s/=[a-z]*/=$(2)/g $(LGE_PRJ_MF))
#endef

#ifeq ($(HW_REVB), TRUE)
#$(call .lge.config.change, CUSTOM_KERNEL_ALSPS, apds9190)
#else
#$(call .lge.config.change, CUSTOM_KERNEL_ALSPS, apds9190)
#endif

ifeq ($(findstring vee5,$(TARGET_PRODUCT)),vee5)
  ifeq ($(HW_REVB), TRUE)
    $(shell mv $(LGE_PRJ_MF) $(LGE_PRJ_MF_TEMP))
    $(shell sed -e /CUSTOM_KERNEL_ALSPS/s/=[a-z0-9]*/=apds9130/g -e /CUSTOM_HAL_MSENSORLIB/s/=[a-z0-9]*/=bmm050/g -e /CUSTOM_KERNEL_ACCELEROMETER/s/=[a-z0-9]*/=bma250/g -e /CUSTOM_KERNEL_MAGNETOMETER/s/=[a-z0-9]*/=bmm050a/g $(LGE_PRJ_MF_TEMP) > $(LGE_PRJ_MF))
    $(shell rm $(LGE_PRJ_MF_TEMP))
  else
    $(shell mv $(LGE_PRJ_MF) $(LGE_PRJ_MF_TEMP))
    $(shell sed -e /CUSTOM_KERNEL_ALSPS/s/=[a-z0-9]*/=apds9130/g -e /CUSTOM_HAL_MSENSORLIB/s/=[a-z0-9]*/=bmc150/g -e /CUSTOM_KERNEL_ACCELEROMETER/s/=[a-z0-9]*/=bma255/g -e /CUSTOM_KERNEL_MAGNETOMETER/s/=[a-z0-9]*/=bmm050/g $(LGE_PRJ_MF_TEMP) > $(LGE_PRJ_MF))
    $(shell rm $(LGE_PRJ_MF_TEMP))
  endif
endif

