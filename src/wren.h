//  Created by Chance Snow on 5/16/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.
#pragma once

#include <stdbool.h>
#include <wren.h>

#include "pd_api.h"

bool initWrenRuntime(PlaydateAPI* pd);
void destroyWrenRuntime(void);
