cmake_minimum_required(VERSION 3.8)

project(XmlParser)

include(${VT_ROOT_PATH}/CMake/SourceFilesGroups.cmake)
include(${VT_ROOT_PATH}/CMake/Module.cmake)

vt_add_module_target(XmlParser STATIC "VT_Engine/Utils")

vt_add_third_party_module(XmlParser PUBLIC "PugiXML" "pugixml::pugixml")

target_link_libraries(XmlParser
	PUBLIC
		Core
)