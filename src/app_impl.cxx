/* Copyright (C) 2012,2013 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include <unistd.h>
#include <sys/types.h>

#include <functional>

#ifdef _WIN32
#else
#include <pwd.h>
#endif

#include <boost/program_options.hpp>

#include "app_impl.h"
#include "config_impl.h"
#include "main-view_impl.h"
#include "main-controller_impl.h"
#include "project-factory_impl.h"
#include "project-ui-factory_impl.h"


NAMESPACE__THITTAM__START

AppImpl::AppImpl ()
{
  logger = m_logger_manager.get ("thittam");
  hipro::log::set_logging_severity (hipro::log::LEVEL_DEBUG3);
}

int
AppImpl::run (int argc, char ** argv)
{
  int error_number;
  if (load_options (argc, argv, &error_number) == false)
    {
      Log_E << "Failed to load run-time options";
      return error_number;
    }

  Glib::signal_idle().connect_once
    (sigc::mem_fun (*this, &AppImpl::create_app_stage1));

  Gtk::Main main (argc, argv);
  Gtk::Main::run ();
  return 0;
}

void
AppImpl::quit (void)
{
  Gtk::Main::quit ();
}

bool
AppImpl::load_options (int argc, char ** argv, int * error_number)
{
  bpop::options_description op_desc ("Allowed options");
  op_desc.add_options ()
    ("help", "Prints this help message")
    ("config", bpop::value<std::string> (), "Path to config file")
    ("server-address", bpop::value<std::string> (), "Server address");

  bpop::variables_map vm;
  try
    {
      bpop::store (bpop::parse_command_line (argc, argv, op_desc), vm);
    }
  catch (std::exception &e)
    {
      std::cout << e.what () << std::endl;
      std::cout << "try --help :)" << std::endl;
      *error_number = 1;
      return false;
    }

  bpop::notify (vm);

  if (vm.count ("help"))
    {
      std::cout << op_desc << std::endl;
      *error_number = 1;
      return false;
    }

  bofs::path config_file = locate_configuration_file ();
  if (vm.count ("config"))
    {
      config_file = vm["config"].as<std::string> ();
    }
  Log_D1 << "Using config-file: " << config_file;

  auto config_source = std::make_unique<FileConfigSource> (logger, config_file);
  m_config = std::make_unique<InMemoryConfig> (logger);
  m_persistent_config =
    std::make_unique<PersistentConfig> (logger, std::move (config_source));

  /* Commandline options will override the persistent-config
   * options. So, we keep the effective runtime-config in a
   * InMemoryConfig object */
  *m_config = *m_persistent_config;

  if (vm.count ("server-address"))
    {
      m_config->set_server_address (vm["server-address"].as<std::string> ());
    }

  *error_number = 0;
  return true;
}

const bofs::path &
AppImpl::locate_configuration_file (void)
{
#ifdef _WIN32
  return locate_configuration_file_w32 ();
#else
  return locate_configuration_file_posix ();
#endif
}

const bofs::path &
AppImpl::locate_home_directory (void)
{
#ifdef _WIN32
  return locate_home_directory_w32 ();
#else
  return locate_home_directory_posix ();
#endif
}

#ifdef _WIN32

const bofs::path &
AppImpl::locate_configuration_file_w32 (void)
{

}

const bofs::path &
AppImpl::locate_home_directory_w32 (void)
{

}

#else  // ifdef _WIN32

const bofs::path &
AppImpl::locate_configuration_file_posix (void)
{
  m_config_file = locate_home_directory_posix ();
  m_config_file /= ".config/thittam/config.info";
  return m_config_file;
}

const bofs::path &
AppImpl::locate_home_directory_posix (void)
{
  struct passwd *pw = getpwuid (getuid ());
  const char * home = pw->pw_dir;
  ASSERT ((home != NULL), "The user does not have a home-directory");
  m_home_directory = home;
  return m_home_directory;
}

#endif  // ifdef _WIN32

void
AppImpl::create_app_stage1 (void)
{
  m_ui_builder =
    Gtk::Builder::create_from_resource ("/ui/main-view.glade");
  auto main_view = std::make_unique<MainViewImpl> (logger, m_ui_builder);
  auto main_controller = std::make_unique<MainControllerImpl> (logger);

  m_project_factory = std::make_unique<ProjectFactoryImpl> (logger);
  auto project_ui_factory = std::make_unique<ProjectUIFactoryImpl> (logger);
  project_ui_factory->set_parent_window (main_view->window ());
  m_project_ui_factory = std::move (project_ui_factory);

  main_controller->set_view (main_view.get ());
  main_controller->set_app (this);
  main_controller->set_project_factory (m_project_factory.get ());
  main_controller->set_project_ui_factory (m_project_ui_factory.get ());

  main_view->set_handler (main_controller.get ());
  main_controller->start ();

  m_main_view = std::move (main_view);
  m_main_controller = std::move (main_controller);
}

void
AppImpl::on_sub_app_closed (SubApp::weak_ptr_t weak_sub_app)
{
  auto sub_app = weak_sub_app.lock ();
  sub_app_list_t::iterator it =
    std::find (m_sub_app_list.begin (), m_sub_app_list.end (), sub_app);
  ASSERT ((it != m_sub_app_list.end ()), "sub-app not in list");

  m_sub_app_list.erase (it);
}

NAMESPACE__THITTAM__END

/*
  Local Variables:
  mode: c++
  End:
*/
