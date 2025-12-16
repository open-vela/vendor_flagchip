/****************************************************************************
 * Copyright (C) 2025 Xiaomi Corporation
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>
#include <nuttx/timers/capture.h>
#include <arch/board/board.h>

#include "chip.h"

#include "fc7300_capture.h"
#include "arm_internal.h"

#include "fc7300f8m-evb.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_FC7300_FTU0_CAP
struct fc7300_callbackmap_s g_ftu0_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu0_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu0_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU1_CAP
struct fc7300_callbackmap_s g_ftu1_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu1_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu1_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU2_CAP
struct fc7300_callbackmap_s g_ftu2_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu2_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu2_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU3_CAP
struct fc7300_callbackmap_s g_ftu3_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu3_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu3_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU4_CAP
struct fc7300_callbackmap_s g_ftu4_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu4_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu4_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU5_CAP
struct fc7300_callbackmap_s g_ftu5_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu5_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu5_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU6_CAP
struct fc7300_callbackmap_s g_ftu6_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu6_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu6_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU7_CAP
struct fc7300_callbackmap_s g_ftu7_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu7_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu7_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU8_CAP
struct fc7300_callbackmap_s g_ftu8_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu8_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu8_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU9_CAP
struct fc7300_callbackmap_s g_ftu9_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu9_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu9_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU10_CAP
struct fc7300_callbackmap_s g_ftu10_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu10_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu10_callback[ch].callback = cbfunc;
    }
}
#endif
#ifdef CONFIG_FC7300_FTU11_CAP
struct fc7300_callbackmap_s g_ftu11_callback[8] =
{
  {
    .channelid   = 0,
    .callback       = NULL,
  },
  {
    .channelid   = 1,
    .callback       = NULL,
  },
  {
    .channelid   = 2,
    .callback       = NULL,
  },
  {
    .channelid   = 3,
    .callback       = NULL,
  },
  {
    .channelid   = 4,
    .callback       = NULL,
  },
  {
    .channelid   = 5,
    .callback       = NULL,
  },
  {
    .channelid   = 6,
    .callback       = NULL,
  },
  {
    .channelid   = 7,
    .callback       = NULL,
  }
};

void fc7300_init_ftu11_callback(uint8_t ch, void *cbfunc)
{
  if (ch < 8)
    {
      g_ftu11_callback[ch].callback = cbfunc;
    }
}
#endif
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_capture_setup
 *
 * Description:
 *   Initialize and register the pwm capture driver.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int fc7300_capture_setup(void)
{
  struct cap_lowerhalf_s *capture;

 #ifdef CONFIG_FC7300_ISM0_CAP

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL0
   capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(0), NULL);
  cap_register("/dev/capture0", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(1), NULL);
  cap_register("/dev/capture1", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(2), NULL);
  cap_register("/dev/capture2", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(3), NULL);
  cap_register("/dev/capture3", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(4), NULL);
  cap_register("/dev/capture4", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(5), NULL);
  cap_register("/dev/capture5", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(6));
  cap_register("/dev/capture6", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(7), NULL);
  cap_register("/dev/capture7", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL8
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(8), NULL);
  cap_register("/dev/capture8", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL9
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(9), NULL);
  cap_register("/dev/capture9", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL10
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(10), NULL);
  cap_register("/dev/capture10", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL11
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(11), NULL);
  cap_register("/dev/capture11", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL12
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(12), NULL);
  cap_register("/dev/capture12", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL13
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(13), NULL);
  cap_register("/dev/capture13", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL14
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(14), NULL);
  cap_register("/dev/capture14", capture);
#endif

#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL15
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_ISM(15), NULL);
  cap_register("/dev/capture15", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU0_CAP

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(0), g_ftu0_callback);
  cap_register("/dev/capture16", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(1), g_ftu0_callback);
  cap_register("/dev/capture17", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(2), g_ftu0_callback);
  cap_register("/dev/capture18", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(3), g_ftu0_callback);
  cap_register("/dev/capture19", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(4), g_ftu0_callback);
  cap_register("/dev/capture20", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(5), g_ftu0_callback);
  cap_register("/dev/capture21", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(6), g_ftu0_callback);
  cap_register("/dev/capture22", capture);
#endif

#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU0(7), g_ftu0_callback);
  cap_register("/dev/capture23", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU1_CAP

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(0), g_ftu1_callback);
  cap_register("/dev/capture24", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(1), g_ftu1_callback);
  cap_register("/dev/capture25", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(2), g_ftu1_callback);
  cap_register("/dev/capture26", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(3), g_ftu1_callback);
  cap_register("/dev/capture27", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(4), g_ftu1_callback);
  cap_register("/dev/capture28", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(5), g_ftu1_callback);
  cap_register("/dev/capture29", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(6), g_ftu1_callback);
  cap_register("/dev/capture30", capture);
#endif

#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU1(7), g_ftu1_callback);
  cap_register("/dev/capture31", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU2_CAP

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(0), g_ftu2_callback);
  cap_register("/dev/capture32", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(1), g_ftu2_callback);
  cap_register("/dev/capture33", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(2), g_ftu2_callback);
  cap_register("/dev/capture34", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(3), g_ftu2_callback);
  cap_register("/dev/capture35", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(4), g_ftu2_callback);
  cap_register("/dev/capture36", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(5), g_ftu2_callback);
  cap_register("/dev/capture37", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(6), g_ftu2_callback);
  cap_register("/dev/capture38", capture);
#endif

#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU2(7), g_ftu2_callback);
  cap_register("/dev/capture39", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU3_CAP

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(0), g_ftu3_callback);
  cap_register("/dev/capture40", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(1), g_ftu3_callback);
  cap_register("/dev/capture41", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(2), g_ftu3_callback);
  cap_register("/dev/capture42", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(3), g_ftu3_callback);
  cap_register("/dev/capture43", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(4), g_ftu3_callback);
  cap_register("/dev/capture44", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(5), g_ftu3_callback);
  cap_register("/dev/capture45", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(6), g_ftu3_callback);
  cap_register("/dev/capture46", capture);
#endif

#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU3(7), g_ftu3_callback);
  cap_register("/dev/capture47", capture);
#endif

 #endif

 #ifdef CONFIG_FC7300_FTU4_CAP

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(0), g_ftu4_callback);
  cap_register("/dev/capture48", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(1), g_ftu4_callback);
  cap_register("/dev/capture49", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(2), g_ftu4_callback);
  cap_register("/dev/capture50", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(3), g_ftu4_callback);
  cap_register("/dev/capture51", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(4), g_ftu4_callback);
  cap_register("/dev/capture52", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(5), g_ftu4_callback);
  cap_register("/dev/capture53", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL6
   capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(6), g_ftu4_callback);
  cap_register("/dev/capture54", capture);
#endif

#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU4(7), g_ftu4_callback);
  cap_register("/dev/capture55", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU5_CAP

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(0), g_ftu5_callback);
  cap_register("/dev/capture56", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(1), g_ftu5_callback);
  cap_register("/dev/capture57", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(2), g_ftu5_callback);
  cap_register("/dev/capture58", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(3), g_ftu5_callback);
  cap_register("/dev/capture59", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(4), g_ftu5_callback);
  cap_register("/dev/capture60", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(5), g_ftu5_callback);
  cap_register("/dev/capture61", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(6), g_ftu5_callback);
  cap_register("/dev/capture62", capture);
#endif

#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU5(7), g_ftu5_callback);
  cap_register("/dev/capture63", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU6_CAP

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(0), g_ftu6_callback);
  cap_register("/dev/capture64", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(1), g_ftu6_callback);
  cap_register("/dev/capture65", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(2), g_ftu6_callback);
  cap_register("/dev/capture66", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(3), g_ftu6_callback);
  cap_register("/dev/capture67", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(4), g_ftu6_callback);
  cap_register("/dev/capture68", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(5), g_ftu6_callback);
  cap_register("/dev/capture69", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(6), g_ftu6_callback);
  cap_register("/dev/capture70", capture);
#endif

#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU6(7), g_ftu6_callback);
  cap_register("/dev/capture71", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU7_CAP

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(0), g_ftu7_callback);
  cap_register("/dev/capture72", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(1), g_ftu7_callback);
  cap_register("/dev/capture73", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(2), g_ftu7_callback);
  cap_register("/dev/capture74", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(3), g_ftu7_callback);
  cap_register("/dev/capture75", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(4), g_ftu7_callback);
  cap_register("/dev/capture76", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(5), g_ftu7_callback);
  cap_register("/dev/capture77", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(6), g_ftu7_callback);
  cap_register("/dev/capture78", capture);
#endif

#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU7(7), g_ftu7_callback);
  cap_register("/dev/capture79", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU8_CAP

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(0), g_ftu8_callback);
  cap_register("/dev/capture80", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(1), g_ftu8_callback);
  cap_register("/dev/capture81", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(2), g_ftu8_callback);
  cap_register("/dev/capture82", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(3), g_ftu8_callback);
  cap_register("/dev/capture83", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(4), g_ftu8_callback);
  cap_register("/dev/capture84", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(5), g_ftu8_callback);
  cap_register("/dev/capture85", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(6), g_ftu8_callback);
  cap_register("/dev/capture86", capture);
#endif

#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU8(7), g_ftu8_callback);
  cap_register("/dev/capture87", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU9_CAP

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(0), g_ftu9_callback);
  cap_register("/dev/capture88", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(1), g_ftu9_callback);
  cap_register("/dev/capture89", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(2), g_ftu9_callback);
  cap_register("/dev/capture90", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(3), g_ftu9_callback);
  cap_register("/dev/capture91", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(4), g_ftu9_callback);
  cap_register("/dev/capture92", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(5), g_ftu9_callback);
  cap_register("/dev/capture93", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(6), g_ftu9_callback);
  cap_register("/dev/capture94", capture);
#endif

#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU9(7), g_ftu9_callback);
  cap_register("/dev/capture95", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU10_CAP

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(0), g_ftu10_callback);
  cap_register("/dev/capture96", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(1), g_ftu10_callback);
  cap_register("/dev/capture97", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(2), g_ftu10_callback);
  cap_register("/dev/capture98", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(3), g_ftu10_callback);
  cap_register("/dev/capture99", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(4), g_ftu10_callback);
  cap_register("/dev/capture100", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(5), g_ftu10_callback);
  cap_register("/dev/capture101", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(6), g_ftu10_callback);
  cap_register("/dev/capture102", capture);
#endif

#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU10(7), g_ftu10_callback);
  cap_register("/dev/capture103", capture);
#endif

#endif

#ifdef CONFIG_FC7300_FTU11_CAP

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL0
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(0), g_ftu11_callback);
  cap_register("/dev/capture104", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL1
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(1), g_ftu11_callback);
  cap_register("/dev/capture105", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL2
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(2), g_ftu11_callback);
  cap_register("/dev/capture106", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL3
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(3), g_ftu11_callback);
  cap_register("/dev/capture107", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL4
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(4), g_ftu11_callback);
  cap_register("/dev/capture108", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL5
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(5), g_ftu11_callback);
  cap_register("/dev/capture109", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL6
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(6), g_ftu11_callback);
  cap_register("/dev/capture110", capture);
#endif

#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL7
  capture = fc7300_cap_initialize(FC7300F8M_EVAL_CAP_FTU11(7), g_ftu11_callback);
  cap_register("/dev/capture111", capture);
#endif

#endif
  return OK;
}
