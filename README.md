test raw malloc and free vs. new and delete of a single byte

to build and run sequentially: <br>
`make run`

Result
-------------------------------------------------------------------------------------- 

consistent CONSTANT overhead for any size for any type.

reason: ***ABI Overhead***. ***Not statically linked***. this means ***one more PLT*** hop:

call -> PLT[_Znwm] -> PLT[malloc] -> ptmalloc2

vs c:

call -> PLT[malloc] -> ptmalloc2

--------------------------------------------------------------------------------------

the difference in percantage highly differs from hardware to hardware:

ArchLinux newest kernel, x86_86 on both:

r5 7600x DDR5 6000mhz cl30: <br>
4.2ns vs 5.5ns, statically linked: 4ns vs 4ns

(laptop) r5 5500u ddr4 3300mhz cl16: <br>
12.7ns vs 13.5ns, statically linked: 12ns vs 12ns (roughly)
