
add_executable(welcome welcome.c welcome.rc)
set_module_type(welcome win32gui UNICODE)
add_importlibs(welcome gdi32 user32 msvcrt kernel32 ntdll)
add_cd_file(TARGET welcome DESTINATION reactos NO_CAB FOR bootcd)
