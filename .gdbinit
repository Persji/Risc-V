set style address foreground white
target remote :1234
load
b main
define f
si
end
define r
c 1000000
end
set disassemble-next-line on
show disassemble-next-line
