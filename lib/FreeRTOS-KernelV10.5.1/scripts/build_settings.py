Import('env')
import os,sys
import json
from os.path import join, realpath
import subprocess

from SCons import Util
import hashlib

if env is None:
    env = {}

def get_library_json():
    f = open(os.getcwd() + "/../library.json")
    jsonconfig = json.load(f)
    return jsonconfig

def get_current_platform():
    current_platform = env.get("PIOPLATFORM", "")
    if current_platform == "native":
        current_platform+= "-" + env.get("PLATFORM","")
    return current_platform


def get_library_common_configuration(jsonconfig):
    library_common_configuration = {}
    if ("build" in jsonconfig):
        if ("environments" in jsonconfig["build"]):
            if ("common" in jsonconfig["build"]["environments"]):
                library_common_configuration = jsonconfig["build"]["environments"]["common"]
    return library_common_configuration

def get_library_platform_configuration(jsonconfig, current_platform):
    library_platform_configuration = {}
    if ("build" in jsonconfig):
        if ("environments" in jsonconfig["build"]):
            if (current_platform in jsonconfig["build"]["environments"]):
                library_platform_configuration = jsonconfig["build"]["environments"][current_platform]
    return library_platform_configuration

# print("library config", library_common_configuration, library_platform_configuration)


def fix_includes(flags):
    for i, f in enumerate(flags):
        # print("i", i, "f", f)
        flag_parts = f.split(" ")
        if (flag_parts[0] == "-I"):
            flags[i] = flag_parts[0] + " " + os.path.abspath("../" + flag_parts[1]).replace(os.sep, "/")
    return flags


def append_library_common_configuration(library_common_configuration):
    if library_common_configuration:
        if ("flags" in library_common_configuration):
            # print("Adding flags",library_common_configuration["flags"])
            fixed_flags = fix_includes(library_common_configuration["flags"])
            # print("fixed", fixed_flags)
            flags = env.ParseFlags(fixed_flags)
            print("flags", flags)
            env.MergeFlags(flags)
            # env.Append(FLAGS=library_common_configuration["flags"])
        if ("srcFilter" in library_common_configuration):
            # print("Adding source filters", library_common_configuration["srcFilter"])
            env.Append(SRC_FILTER=library_common_configuration["srcFilter"])

def append_library_platform_configuration(library_platform_configuration):
    if library_platform_configuration:
        if ("flags" in library_platform_configuration):
            # print("Adding flags",library_platform_configuration["flags"])
            fixed_flags = fix_includes(library_platform_configuration["flags"])
            # print("fixed", fixed_flags)
            flags = env.ParseFlags(fixed_flags)
            # print("flags", flags)
            env.MergeFlags(flags)
            # env.Append(FLAGS=library_platform_configuration["flags"])
        if ("srcFilter" in library_platform_configuration):
            # print("Adding source filters", library_platform_configuration["srcFilter"])
            env.Append(SRC_FILTER=library_platform_configuration["srcFilter"])


print("platform", get_current_platform())
jsonconfig = get_library_json()
append_library_common_configuration(get_library_common_configuration(jsonconfig))
append_library_platform_configuration(get_library_platform_configuration(jsonconfig, get_current_platform()))

# print(env.Dump())