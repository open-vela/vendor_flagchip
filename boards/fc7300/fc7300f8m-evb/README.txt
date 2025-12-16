README
======

This directory holds the port to the Flagchip FC7300F8M evb board.

Contents
========

  o Status
  o Serial Console
  o LEDs and Buttons
  o Configurations

Status
======


Serial Console
==============

  By default, the serial console will be provided onboard usb to serial port:

    UART RX  PTA31  (FCUART2_RX)
    UART TX  PTA9   (FCUART2_TX)

LEDs and Buttons
================


Configurations
==============

  Common Information
  ------------------
  Each FC4150F1M-ESMALL configuration is maintained in a sub-directory and can
  be selected as follows:

    tools/configure.sh fc4150f1m-esmall:<subdir>

  Where <subdir> is one of the sub-directories listed in the next paragraph.

    NOTES (common for all configurations):

    1. This configuration uses the mconf-based configuration tool.  To change
       this configuration using that tool, you should:

       a. Build and install the kconfig-mconf tool.  See nuttx/README.txt.
          Also see additional README.txt files in the NuttX tools repository.

       b. Execute 'make menuconfig' in nuttx/ in order to start the
          reconfiguration process.

    2. Unless otherwise stated, the serial console used is FCUART2 at
       115,200 8N1.

  Configuration Sub-directories
  -----------------------------

    nsh:
    ---
      Configures the NuttShell (nsh) located at apps/examples/nsh.  Support
      for builtin applications is enabled, but in the base configuration the
      only application selected is the "Hello, World!" example.

    1. This example supports the PWM test (apps/examples/pwm) but this must
       be manually enabled by selecting either

       CONFIG_PWM=y                : Enable the generic PWM infrastructure
       CONFIG_FC4150_FTU0=y        : Enable the FTU0 hardware units
       CONFIG_FC4150_FTU0_PWM=y    : Enable the FTU0 PWM function
       CONFIG_FC4150_FTU0_PWM_CHANNEL2=y : Enable the FTU0 channel2
       CONFIG_FC4150_PWM=y         : Enable the generic PWM infrastructure

       Or..

       CONFIG_PWM=y                : Enable the generic PWM infrastructure
       CONFIG_PWM_MULTICHAN=y      : Enable the multiple channel infrastructure
       CONFIG_PWM_NCHANNELS=2      : number of channels
       CONFIG_FC4150_FTU0=y        : Enable the FTU0 hardware units
       CONFIG_FC4150_FTU0_PWM=y    : Enable the FTU0 PWM function
       CONFIG_FC4150_FTU0_PWM_CHANNEL2=y : Enable the FTU0 channel2
       CONFIG_FC4150_FTU0_PWM_CHANNEL3=y : Enable the FTU0 channel3
       CONFIG_FC4150_PWM=y         : Enable the generic PWM infrastructure
       CONFIG_FC4150_PWM_MULTICHAN=y : Enable the multiple channel mode

       See also include/board.h and apps/examples/README.txt

       Special PWM-only debug options:

       CONFIG_DEBUG_PWM_INFO
