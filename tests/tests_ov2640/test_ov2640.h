#ifndef TEST_OV2640
#define TEST_OV2640

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../ov2640/ov2640.h"
#include "../../mocks/hal_mock.h"

void run_ov2640_tests(void);

#endif // TEST_OV2640