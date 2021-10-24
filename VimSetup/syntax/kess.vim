syntax match OUT /^out\>/
syntax match BAK /^bak\>/
syntax match INT_KEYWORD /^int\>/
syntax match COMMENT /\/\/.*/
syntax region STRING start=/"/ skip=/\\"/ end=/"/
syntax match INTEGER /[0-9]/
syntax match FILE_READ_OUT /^__file_read_out__\>/
syntax match STRING_KEYWORD /^str\>/
syntax match c_start /C_START:\>/
syntax match c_end /C_END\>/
