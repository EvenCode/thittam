/* Copyright (C) 2008-2018 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__f43a3a02_438f_11e8_97cc_448500dfb04c
#define HIPRO__f43a3a02_438f_11e8_97cc_448500dfb04c

#include "_namespace.h"

NAMESPACE__THITTAM__START

class ProjectController
{
public:
  virtual ~ProjectController () {}

  virtual bool dirty (void) const = 0;
  virtual void load (void) const = 0;
  virtual void save (const std::string & file_name) const = 0;

  virtual void attach_ui (void) const = 0;
  virtual void detach_ui (void) const = 0;
};

NAMESPACE__THITTAM__END

#endif // HIPRO__f43a3a02_438f_11e8_97cc_448500dfb04c

/*
  Local Variables:
  mode: c++
  End:
*/
