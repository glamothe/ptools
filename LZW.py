#Lempel-Ziv-Welch compression algorithm
#Translated to python by Thomas Van Durme
#Last edited: September 20, 2006
class LZW:
    """Returns the compressed string in utf8 format"""
    def compress(self,uncompressed):
        if isinstance(uncompressed, str):
            chars = int(256)
            mydict = dict()
            buffer = list()
            result = str()
            for i in range(chars):
                mydict[str(i)] = i
            for i in uncompressed:
                if len(buffer) == 0:
                    xstr = str(ord(i))
                else:
                    xstr = self.__join(buffer,"-")+"-"+str(ord(i))
                if mydict.has_key(xstr):
                    buffer.append(ord(i))
                else:
                    result += unichr(mydict[self.__join(buffer,"-")]).encode('utf8')
                    mydict[xstr] = chars
                    chars += 1
                    del buffer
                    buffer = list()
                    buffer.append(ord(i))
            if len(buffer) != 0:
                result += unichr(mydict[self.__join(buffer,"-")]).encode('utf8')
            return result
        else:
            raise TypeError
 
    """Returns the decompressed string, input is a utf8 compressed string"""
    def decompress(self,compressed):
        if isinstance(compressed, str):
            chars = int(256)
            mydict = dict()
            for i in range(chars):
                mydict[i] = unichr(i).encode('utf8')
            decoded = compressed.decode('string_escape').decode('utf8')
            buffer = str()
            chain = str()
            result = str()
            for i in decoded:
                code = ord(i)
                if buffer == "":
                    current = mydict[code]
                    buffer = current
                    result += current
                else:
                    if code<=255:
                        current = mydict[code]
                        result += current
                        chain = buffer+current
                        mydict[chars] = chain
                        chars += 1
                        buffer = current
                    else:
                        if mydict.has_key(code):
                            chain = mydict[code]
                        else:
                            chain = buffer+buffer[0]
                        result += chain
                        mydict[chars] = buffer+chain[0]
                        chars += 1
                        buffer = chain
            return result
        else:
            raise TypeError
 
    def __join(self,mylist,delimiter):
        if isinstance(mylist,list) and isinstance(delimiter,str):
            result = str()
            for i in range(0,len(mylist)):
                try:
                    if i+1 == len(mylist):
                        result += str(mylist[i])
                    else:
                        result += str(mylist[i])+delimiter
                except TypeError:
                    pass
            return result
        else:
            raise TypeError
