# PA1-Resubmission Qeustion:
    For 11100000 10000000 10100001: 
        11100000 (1110 xxxx), indicates its a 3 byte UTF8 sequence
        10000000 (10xx xxxx), is continuation byte.
        10100001 (10xx xxxx), is continuation byte too, but have the info vaules (10 0001).
    Thus: 
        100001 binary -> 33 dec -> 21 hex
    To combin these, the code point is 0x21, the character is "!".

    The other ways to encode are : Decimal 
    

# PA1-Q1:

    UTF-8 encoding is more compact and has the advantage of saving storage space for big data compared to UTF-32. However, the variable storage length of characters makes access or query operations more complicated.

    UTF-32 encoding has a fixed length, which makes operations simple but wastes a lot of storage space. Especially when storing a large amount of ASCII code.

# PA2-Q2:
    Using the leading 10 makes it easier to identify consecutive bytes and prevents confusion with other starting bytes. In addition, it has a certain error prevention function. Since the transmitted data may be interfered by the environment. The leading 10 can easily identify these errors.

    Without the leading 10, it will be difficult to identify the beginning and end of multi-byte characters. And the ability to correct erroneous bytes is lost.


