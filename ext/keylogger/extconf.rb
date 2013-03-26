require 'mkmf'

have_header('linux/input.h')

$warnflags = ''
$defs.push '-D_GNU_SOURCE'
$defs.push '-Wall'

create_makefile('keylogger')
