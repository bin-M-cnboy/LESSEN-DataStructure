import sys

def main():
    lines = sys.stdin.read(). strip().split('\n')
    
    # 第一遍：解析标签，构建符号表
    symbol_table = {}
    instructions = []
    orig_addr = 0
    current_addr = 0
    
    for line in lines:
        # 去除注释和首尾空白
        if ';' in line:
            line = line[:line.index(';')]
        line = line.strip()
        if not line:
            continue
        
        # 解析指令
        parts = []
        temp = ''
        in_string = False
        for ch in line:
            if ch == '"':
                in_string = not in_string
                temp += ch
            elif ch in ' ,\t' and not in_string:
                if temp:
                    parts.append(temp)
                    temp = ''
            else:
                temp += ch
        if temp:
            parts.append(temp)
        
        if not parts:
            continue
        
        # 处理 .ORIG
        if parts[0]. upper() == '.ORIG':
            orig_addr = parse_number(parts[1])
            current_addr = orig_addr
            continue
        
        # 处理 .END
        if parts[0].upper() == '.END':
            break
        
        # 检查是否有标签
        first = parts[0]
        if first.endswith(':'):
            label = first[:-1]
            symbol_table[label] = current_addr
            parts = parts[1:]
            if not parts:
                continue
        elif first.upper() not in get_opcodes() and not first.startswith('.'):
            # 无冒号的标签
            symbol_table[first] = current_addr
            parts = parts[1:]
            if not parts:
                continue
        
        if not parts:
            continue
        
        op = parts[0]. upper()
        
        # 计算指令占用的字数
        if op == '. FILL':
            instructions.append((current_addr, parts))
            current_addr += 1
        elif op == '. BLKW':
            count = parse_number(parts[1])
            instructions.append((current_addr, parts))
            current_addr += count
        elif op == '.STRINGZ':
            # 找到字符串
            idx = line.upper().find('. STRINGZ')
            rest = line[idx + 8:].strip()
            # 提取引号内的字符串
            start = rest.index('"') + 1
            end = rest.rindex('"')
            s = rest[start:end]
            s = process_escape(s)
            instructions.append((current_addr, ['. STRINGZ', s]))
            current_addr += len(s) + 1  # 包含 null 终止符
        else:
            instructions.append((current_addr, parts))
            current_addr += 1
    
    # 第二遍：生成机器码
    output = []
    output.append(format(orig_addr, '016b'))
    
    for addr, parts in instructions:
        op = parts[0]. upper()
        
        if op == '. FILL':
            val = parse_value(parts[1], symbol_table)
            output.append(format(val & 0xFFFF, '016b'))
        elif op == '.BLKW':
            count = parse_number(parts[1])
            for _ in range(count):
                output.append('0000000000000000')
        elif op == '. STRINGZ':
            s = parts[1]
            for ch in s:
                output.append(format(ord(ch), '016b'))
            output.append('0000000000000000')  # null 终止符
        elif op == 'ADD':
            output.append(assemble_add(parts))
        elif op == 'AND':
            output. append(assemble_and(parts))
        elif op == 'BR' or op == 'BRN' or op == 'BRZ' or op == 'BRP' or \
             op == 'BRNZ' or op == 'BRNP' or op == 'BRZP' or op == 'BRNZP':
            output.append(assemble_br(parts, addr, symbol_table))
        elif op == 'JMP':
            output.append(assemble_jmp(parts))
        elif op == 'RET':
            output.append('1100000111000000')
        elif op == 'JSR':
            output.append(assemble_jsr(parts, addr, symbol_table))
        elif op == 'JSRR':
            output.append(assemble_jsrr(parts))
        elif op == 'LD':
            output.append(assemble_ld(parts, addr, symbol_table))
        elif op == 'LDI':
            output. append(assemble_ldi(parts, addr, symbol_table))
        elif op == 'LDR':
            output.append(assemble_ldr(parts))
        elif op == 'LEA':
            output.append(assemble_lea(parts, addr, symbol_table))
        elif op == 'NOT':
            output. append(assemble_not(parts))
        elif op == 'RTI':
            output.append('1000000000000000')
        elif op == 'ST':
            output. append(assemble_st(parts, addr, symbol_table))
        elif op == 'STI':
            output.append(assemble_sti(parts, addr, symbol_table))
        elif op == 'STR':
            output.append(assemble_str(parts))
        elif op == 'TRAP':
            output.append(assemble_trap(parts))
        elif op == 'GETC':
            output.append('1111000000100000')
        elif op == 'OUT':
            output. append('1111000000100001')
        elif op == 'PUTS':
            output. append('1111000000100010')
        elif op == 'IN':
            output. append('1111000000100011')
        elif op == 'PUTSP':
            output.append('1111000000100100')
        elif op == 'HALT':
            output.append('1111000000100101')
    
    print('\n'.join(output))


def get_opcodes():
    return {'ADD', 'AND', 'BR', 'BRN', 'BRZ', 'BRP', 'BRNZ', 'BRNP', 'BRZP', 'BRNZP',
            'JMP', 'JSR', 'JSRR', 'LD', 'LDI', 'LDR', 'LEA', 'NOT', 'RET', 'RTI',
            'ST', 'STI', 'STR', 'TRAP', 'GETC', 'OUT', 'PUTS', 'IN', 'PUTSP', 'HALT'}


def parse_number(s):
    s = s.strip()
    if s. startswith('x') or s.startswith('X'):
        return int(s[1:], 16)
    elif s.startswith('#'):
        return int(s[1:])
    elif s.startswith('-'):
        return int(s)
    else:
        return int(s)


def parse_value(s, symbol_table):
    s = s.strip()
    if s in symbol_table:
        return symbol_table[s]
    return parse_number(s)


def parse_register(s):
    s = s.strip(). upper()
    return int(s[1])


def process_escape(s):
    result = ''
    i = 0
    while i < len(s):
        if s[i] == '\\' and i + 1 < len(s):
            c = s[i + 1]
            if c == 'n':
                result += '\n'
            elif c == 't':
                result += '\t'
            elif c == '\\':
                result += '\\'
            elif c == '"':
                result += '"'
            else:
                result += s[i + 1]
            i += 2
        else:
            result += s[i]
            i += 1
    return result


def to_twos_complement(val, bits):
    if val < 0:
        val = (1 << bits) + val
    return val & ((1 << bits) - 1)


def assemble_add(parts):
    dr = parse_register(parts[1])
    sr1 = parse_register(parts[2])
    if parts[3]. upper(). startswith('R'):
        sr2 = parse_register(parts[3])
        return format((0b0001 << 12) | (dr << 9) | (sr1 << 6) | sr2, '016b')
    else:
        imm5 = parse_number(parts[3])
        imm5 = to_twos_complement(imm5, 5)
        return format((0b0001 << 12) | (dr << 9) | (sr1 << 6) | (1 << 5) | imm5, '016b')


def assemble_and(parts):
    dr = parse_register(parts[1])
    sr1 = parse_register(parts[2])
    if parts[3].upper().startswith('R'):
        sr2 = parse_register(parts[3])
        return format((0b0101 << 12) | (dr << 9) | (sr1 << 6) | sr2, '016b')
    else:
        imm5 = parse_number(parts[3])
        imm5 = to_twos_complement(imm5, 5)
        return format((0b0101 << 12) | (dr << 9) | (sr1 << 6) | (1 << 5) | imm5, '016b')


def assemble_br(parts, addr, symbol_table):
    op = parts[0]. upper()
    n = 1 if 'N' in op[2:] else 0
    z = 1 if 'Z' in op[2:] else 0
    p = 1 if 'P' in op[2:] else 0
    if op == 'BR':
        n = z = p = 1
    target = parse_value(parts[1], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b0000 << 12) | (n << 11) | (z << 10) | (p << 9) | offset, '016b')


def assemble_jmp(parts):
    baser = parse_register(parts[1])
    return format((0b1100 << 12) | (baser << 6), '016b')


def assemble_jsr(parts, addr, symbol_table):
    target = parse_value(parts[1], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 11)
    return format((0b0100 << 12) | (1 << 11) | offset, '016b')


def assemble_jsrr(parts):
    baser = parse_register(parts[1])
    return format((0b0100 << 12) | (baser << 6), '016b')


def assemble_ld(parts, addr, symbol_table):
    dr = parse_register(parts[1])
    target = parse_value(parts[2], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b0010 << 12) | (dr << 9) | offset, '016b')


def assemble_ldi(parts, addr, symbol_table):
    dr = parse_register(parts[1])
    target = parse_value(parts[2], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b1010 << 12) | (dr << 9) | offset, '016b')


def assemble_ldr(parts):
    dr = parse_register(parts[1])
    baser = parse_register(parts[2])
    offset = parse_number(parts[3])
    offset = to_twos_complement(offset, 6)
    return format((0b0110 << 12) | (dr << 9) | (baser << 6) | offset, '016b')


def assemble_lea(parts, addr, symbol_table):
    dr = parse_register(parts[1])
    target = parse_value(parts[2], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b1110 << 12) | (dr << 9) | offset, '016b')


def assemble_not(parts):
    dr = parse_register(parts[1])
    sr = parse_register(parts[2])
    return format((0b1001 << 12) | (dr << 9) | (sr << 6) | 0x3F, '016b')


def assemble_st(parts, addr, symbol_table):
    sr = parse_register(parts[1])
    target = parse_value(parts[2], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b0011 << 12) | (sr << 9) | offset, '016b')


def assemble_sti(parts, addr, symbol_table):
    sr = parse_register(parts[1])
    target = parse_value(parts[2], symbol_table)
    offset = target - (addr + 1)
    offset = to_twos_complement(offset, 9)
    return format((0b1011 << 12) | (sr << 9) | offset, '016b')


def assemble_str(parts):
    sr = parse_register(parts[1])
    baser = parse_register(parts[2])
    offset = parse_number(parts[3])
    offset = to_twos_complement(offset, 6)
    return format((0b0111 << 12) | (sr << 9) | (baser << 6) | offset, '016b')


def assemble_trap(parts):
    trapvect = parse_number(parts[1])
    return format((0b1111 << 12) | trapvect, '016b')


if __name__ == '__main__':
    main()