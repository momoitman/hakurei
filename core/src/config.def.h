#pragma once

// This file is for customized build for hakurei.
// Most variables should be defined as preprocessor macro
// Some variables may become dynamically configureable one day maybe?

// This file is default values for the variables.
// All values must be corresponding against config.h, and be appended with suffix _DEF

#define HAKUREI_PERSISTENCE_NAME csv_persistence
#define HAKUREI_PERSISTENCE_INIT_DEF(name, table_spec) name, table_spec, "./data"

#ifndef HAKUREI_PERSISTENCE_INIT
#define HAKUREI_PERSISTENCE_INIT HAKUREI_PERSISTENCE_INIT_DEF
#endif