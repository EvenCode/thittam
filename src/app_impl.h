/* Copyright (C) 2012,2013 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO_THITTAM__3ee00958_fd93_11e2_b7e5_001f3c9e2082
#define HIPRO_THITTAM__3ee00958_fd93_11e2_b7e5_001f3c9e2082

#include <memory>
#include <boost/filesystem.hpp>

#include "log.h"
#include "common.h"
#include "app.h"
#include "config.h"
#include "main-view.h"
#include "main-controller.h"
#include "project-factory.h"
#include "project-ui-factory.h"

NAMESPACE__THITTAM__START

namespace bofs = ::boost::filesystem;

class AppImpl : public App,
                public std::enable_shared_from_this<AppImpl>
{
public:
  typedef ::std::shared_ptr<AppImpl> ptr_t;

public:
  /*--- ctor/dtor/factory ---*/
  AppImpl ();
  ~AppImpl () {}

  /*--- Methods required by the App interface ---*/
  int run (int argc, char **argv);
  void quit (void);

  /*--- Other convenience methods ---*/
  bool load_options (int argc, char ** argv, int * error_number);
  const bofs::path & locate_configuration_file (void);
  const bofs::path & locate_home_directory (void);
#ifdef _WIN32
  const bofs::path & locate_configuration_w32 (void);
  const bofs::path & locate_home_directory_w32 (void);
#else
  const bofs::path & locate_configuration_file_posix (void);
  const bofs::path & locate_home_directory_posix (void);
#endif

  void create_app_stage1 (void);
  void on_sub_app_closed (SubApp::weak_ptr_t sub_app);

  /*--- Data members ---*/
  hipro::log::Logger* logger;
  hipro::log::LoggerManager m_logger_manager;
  bofs::path m_home_directory;
  bofs::path m_config_file;
  std::unique_ptr<Config> m_config;
  std::unique_ptr<Config> m_persistent_config;

  Glib::RefPtr<Gtk::Builder> m_ui_builder;
  std::unique_ptr<MainView> m_main_view;
  std::unique_ptr<MainController> m_main_controller;
  std::unique_ptr<ProjectFactory> m_project_factory;
  std::unique_ptr<ProjectUIFactory> m_project_ui_factory;

  typedef std::vector<SubApp::ptr_t> sub_app_list_t;
  sub_app_list_t m_sub_app_list;
};

NAMESPACE__THITTAM__END

#endif // HIPRO_THITTAM__3ee00958_fd93_11e2_b7e5_001f3c9e2082

/*
  Local Variables:
  mode: c++
  End:
*/
