/* Copyright (C) 2008-2018 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__08c4ab2a_2431_11e8_9818_448500dfb04c
#define HIPRO__08c4ab2a_2431_11e8_9818_448500dfb04c

#include <string>
#include <cstdlib>
#include <ctime>

#define MOD_MAX 999999

#define NAMESPACE__THITTAM_UTIL__START namespace thittam { namespace util {
#define NAMESPACE__THITTAM_UTIL__END } }

NAMESPACE__THITTAM_UTIL__START

void init_random (void);

int effort_str_to_minutes (
  const std::string & effort, int working_days_per_week,
  int working_hours_per_day);

// generates 6 digit random number string for resource and group id.
unsigned int generate_random_id ();

NAMESPACE__THITTAM_UTIL__END

#endif // HIPRO__08c4ab2a_2431_11e8_9818_448500dfb04c

/*
  Local Variables:
  mode: c++
  End:
*/
