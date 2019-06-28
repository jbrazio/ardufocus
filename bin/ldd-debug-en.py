#
# Add additonal linker flag:
# Generate debugging information
#
Import('env')
env.Append(LINKFLAGS=["-g"])
