for %%f in (%1) do copy %%f %%f.utf8
for %%f in (%1) do iconv -c -f utf-8 -t gb2312 %%f.utf8 > %%f
for %%f in (%1) do del %%f.utf8