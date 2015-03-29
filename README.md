#概述

2011年上半年，江苏电信更换了中国矿业大学学生宿舍的宽带认证系统。新的认证过程采用了PPPOE协议，并在密码字段使用了加密密文，本白皮书正是为了讲述此加密算法的详细流程。

#加密算法

## 算法概述

此算法主要使用了DES加密算法，同时也使用了一种类似MD5的算法和BASE64编码算法。
宽带认证用户必须提供的密码是上网卡号密码（8位）或者天翼手机的手机密码（6位），其加密后的密文长116字节。

密文不可以重复使用，即每次认证都需要重新计算密文，因为密文有一部分是随机产生的，服务端不允许同一密文在一定时间里多次使用。虽然服务端密文记录重置时间不明确，但是只要每次都重新计算即可确保无虞。

本算法是通过反汇编qlongyinqw写的Linux下密文计算程序并经过笔者整理分析而获知，由于不能直接获取官方认证程序的算法，所以本文中所提的细节并不一定与官方认证程序完全相同，但将其用于宽带认证是确实可行的。

## 算法详述

### 密文构成

密文由四个部分拼接构成，共116字节。

- 固定字符串，22字节，”824ef19fb7ff1afanji^3!”。
- 随机部分，76字节，见下文描述。
- 固定字符串，6字节，”i0~!8^”。
- 密码部分，12字节，见下文描述。

### 随机部分

随机部分是服务端为了避免密文重用，其算法流程如下。
    
生成18字节的随机字符串，qlongyinqw写的密文计算程序中使用的是用户名尾部8位及两个随机5位整数，随机整数是由C标准库中的rand函数生成的随机数经某种算法计算而来。而实际上此随机字符串的要求并不严格，虽然不能确切了解到字符值的范围，但至少拉丁字母和阿拉伯数字和个别特殊字符可以的，不必细究，不妨令其为18个随机’0’至’9’之间的字符。

将此18字节经一种类似MD5的算法计算信息摘要，并将16字节信息摘要以大写十六进制编码输出（32字节）。此算法其他部分与MD5一样，不同的是此算法将MD5算法中F、G、H、I变换的顺序更改为G、F、I、H，而加法常数的使用顺序不变，即G变换使用的是前64个加法常数，F变换使用的是接下来64个，I变换、H变换依此类推。

拼接一个56字节的字符串，由三部分组成，原先的18字节随机字符串、6字节固定字符串”*3^F$k”、类似MD5算法的信息摘要32字节字符串。

将此56字节经DES加密，其加（解）密密钥见第三章，结果以BASE64编码输出，共76字节。

### 密码部分

将用户密码以空格进行对齐，即如果密码少于8字节则在后面拼接空格补齐8字节，然后进行DES加密，其加（解）密密钥见第三章，结果以BASE64编码输出，共12字节。

## 关于DES密钥

由于DES的初始密钥是8字节，其中每个字节的最低位作为奇偶校验位在实际加密、解密中不起作用，而笔者只能通过反汇编qlongyinqw写的密码计算程序得到768位的子密钥，所以经过密钥生成算法的逆运算算得的密钥有256种可能的版本。

密钥可能的取值如下：

### 随机部分DES加密密钥

<table>
    <tr>
        <td>字符串索引</td>
        <td>0</td>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
        <td>5</td>
        <td>6</td>
        <td>7</td>
    </tr>
    <tr>
        <td rowspan="2">可能的取值</td>
        <td>`</td>
        <td>b</td>
        <td>b</td>
        <td>d</td>
        <td>d</td>
        <td>f</td>
        <td>f</td>
        <td>h</td>
    </tr>
    <tr>
        <td>a</td>
        <td>c</td>
        <td>c</td>
        <td>e</td>
        <td>e</td>
        <td>g</td>
        <td>g</td>
        <td>i</td>
    </tr>
</table>	
	
实际上可以推断出，官方的原认证程序使用的密钥应该就是”abcdefgh”。

### 密码部分DES加密密钥

<table>
    <tr>
        <td>字符串索引</td>
        <td>0</td>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
        <td>5</td>
        <td>6</td>
        <td>7</td>
    </tr>
    <tr>
        <td rowspan="2">可能的取值</td>
        <td>0</td>
        <td>p</td>
        <td>`</td>
        <td>z</td>
        <td>2</td>
        <td>v</td>
        <td>r</td>
        <td>x</td>
    </tr>
    <tr>
        <td>1</td>
        <td>q</td>
        <td>a</td>
        <td>{</td>
        <td>3</td>
        <td>w</td>
        <td>s</td>
        <td>y</td>
    </tr>
</table>	
	
可以看出原密钥很有可能是两个人名的首写字母，不必细究，实际使用过程中每一字节可以任取两者之一。




