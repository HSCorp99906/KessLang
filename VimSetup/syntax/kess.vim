syntax match OUT /out/
syntax match BAK /bak/
syntax match INT_KEYWORD /int/
syntax region STRING start=/"/ skip=/\\"/ end=/"/
syntax match COMMENT /\/\/.*/
syntax match INTEGER /[0-9]/
