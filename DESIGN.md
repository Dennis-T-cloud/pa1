# PA1-Resubmission Qeustion:
    a) For 11100000 10000000 10100001: 
        11100000 (1110 xxxx), indicates its a 3 byte UTF8 sequence
        10000000 (10xx xxxx), is continuation byte.
        10100001 (10xx xxxx), is continuation byte too, but have the vaules (10 0001).
    Thus: 
        100001 binary -> 33 dec -> 21 hex
    To combin these, the code point is U+0021 / 33, the character is "!".

    b) The other three ways to encode "!" are :   
        ASCII : 0x21
        utf-16: Big-endian: 00 21
                little-endian: 21 00     
        utf-32: Big-endian: 00 00 00 21
                little-endian: 21 00 00 00

    c) An example of a character that has exactly three encodings:
        An example is:
            "non-breaking space character", U+00A0.
        It does not have an ASCII encoding because it exceeds the ASCII set (U+0000 to U+007F).

        UTF-8 : C2 A0
        UTF-16: 00 A0 / A0 00 (Big-endian / Little-endian)
        UTF-32: 00 00 00 A0 / A0 00 00 00
    
    d) The existence of multiple encodings for the same character introduces a plethora of significant security and interoperability problems. These overlong encodings can be exploited to sneak past security filters that don't recognize these unconventional forms, thereby allowing malicious inputs to dodge detection mechanisms. This loophole can lead to vulnerabilities such as code injection and cross-site scripting attacks, where attackers execute harmful scripts on unsuspecting users' systems. 

    Additionally, having multiple encodings causes data inconsistencies because different systems might interpret the same character in varying ways depending on its encoding, which is a real headache. This not only ramps up processing overhead due to the need for normalization but also complicates efforts to maintain data integrity. To keep security tight and data consistent, it's crucial to adhere strictly to standard encoding practices and flat-out reject those troublesome overlong sequences. After all, we definitely don't want to open the door to unexpected vulnerabilities and the chaos they bring along.
    
    Cited info: Kevin Boone, "UTF-8 and the problem of over-long characters",  https://kevinboone.me/overlong.html





