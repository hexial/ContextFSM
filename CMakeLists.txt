cmake_minimum_required (VERSION 3.18)

project (ContextFSM)

add_library(ContextFSM INTERFACE
	)
	
target_include_directories(ContextFSM
	PUBLIC
	INTERFACE
	src
)

add_library(ContextFSM::ContextFSM ALIAS ContextFSM)
