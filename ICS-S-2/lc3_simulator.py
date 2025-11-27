import sys

def main():
    lines = sys.stdin.read(). strip().split('\n')
    
    # 分离代码和查询
    code_lines = []
    query_lines = []
    in_query = False
    
    for line in lines:
        if line == '':
            in_query = True
            continue
        if in_query:
            query_lines.append(line)
        else:
            code_lines. append(line)
    
    # 初始化内存和寄存器
    memory = [0] * 65536
    registers = [0] * 8
    cond = 2  # Z flag (N=4, Z=2, P=1)
    
    # 装载程序
    pc = int(code_lines[0], 2)
    start_pc = pc
    for i in range(1, len(code_lines)):
        memory[start_pc + i - 1] = int(code_lines[i], 2)
    
    # 辅助函数
    def sext(val, bits):
        if val & (1 << (bits - 1)):
            return val - (1 << bits)
        return val
    
    def to_unsigned(val):
        return val & 0xFFFF
    
    def update_flags(val):
        nonlocal cond
        val = to_unsigned(val)
        if val == 0:
            cond = 2
        elif val & 0x8000:
            cond = 4
        else:
            cond = 1
    
    # 执行程序
    while True:
        instr = memory[pc]
        pc = (pc + 1) & 0xFFFF
        opcode = (instr >> 12) & 0xF
        
        if opcode == 0:  # BR
            n = (instr >> 11) & 1
            z = (instr >> 10) & 1
            p = (instr >> 9) & 1
            offset = sext(instr & 0x1FF, 9)
            if (n and cond == 4) or (z and cond == 2) or (p and cond == 1):
                pc = (pc + offset) & 0xFFFF
        elif opcode == 1:  # ADD
            dr = (instr >> 9) & 7
            sr1 = (instr >> 6) & 7
            if instr & 0x20:
                imm5 = sext(instr & 0x1F, 5)
                registers[dr] = to_unsigned(registers[sr1] + imm5)
            else:
                sr2 = instr & 7
                registers[dr] = to_unsigned(registers[sr1] + registers[sr2])
            update_flags(registers[dr])
        elif opcode == 2:  # LD
            dr = (instr >> 9) & 7
            offset = sext(instr & 0x1FF, 9)
            registers[dr] = memory[(pc + offset) & 0xFFFF]
            update_flags(registers[dr])
        elif opcode == 3:  # ST
            sr = (instr >> 9) & 7
            offset = sext(instr & 0x1FF, 9)
            memory[(pc + offset) & 0xFFFF] = registers[sr]
        elif opcode == 4:  # JSR/JSRR
            registers[7] = pc
            if instr & 0x800:
                offset = sext(instr & 0x7FF, 11)
                pc = (pc + offset) & 0xFFFF
            else:
                baser = (instr >> 6) & 7
                pc = registers[baser]
        elif opcode == 5:  # AND
            dr = (instr >> 9) & 7
            sr1 = (instr >> 6) & 7
            if instr & 0x20:
                imm5 = sext(instr & 0x1F, 5)
                registers[dr] = to_unsigned(registers[sr1] & imm5)
            else:
                sr2 = instr & 7
                registers[dr] = to_unsigned(registers[sr1] & registers[sr2])
            update_flags(registers[dr])
        elif opcode == 6:  # LDR
            dr = (instr >> 9) & 7
            baser = (instr >> 6) & 7
            offset = sext(instr & 0x3F, 6)
            registers[dr] = memory[(registers[baser] + offset) & 0xFFFF]
            update_flags(registers[dr])
        elif opcode == 7:  # STR
            sr = (instr >> 9) & 7
            baser = (instr >> 6) & 7
            offset = sext(instr & 0x3F, 6)
            memory[(registers[baser] + offset) & 0xFFFF] = registers[sr]
        elif opcode == 8:  # RTI
            pass  # 用户模式忽略
        elif opcode == 9:  # NOT
            dr = (instr >> 9) & 7
            sr = (instr >> 6) & 7
            registers[dr] = to_unsigned(~registers[sr])
            update_flags(registers[dr])
        elif opcode == 10:  # LDI
            dr = (instr >> 9) & 7
            offset = sext(instr & 0x1FF, 9)
            addr = memory[(pc + offset) & 0xFFFF]
            registers[dr] = memory[addr]
            update_flags(registers[dr])
        elif opcode == 11:  # STI
            sr = (instr >> 9) & 7
            offset = sext(instr & 0x1FF, 9)
            addr = memory[(pc + offset) & 0xFFFF]
            memory[addr] = registers[sr]
        elif opcode == 12:  # JMP/RET
            baser = (instr >> 6) & 7
            pc = registers[baser]
        elif opcode == 14:  # LEA
            dr = (instr >> 9) & 7
            offset = sext(instr & 0x1FF, 9)
            registers[dr] = (pc + offset) & 0xFFFF
            update_flags(registers[dr])
        elif opcode == 15:  # TRAP
            if (instr & 0xFF) == 0x25:  # HALT
                break
    
    # 回答查询
    for q in query_lines:
        if q. startswith('R'):
            reg_num = int(q[1])
            print(f"x{registers[reg_num]:04x}")
        elif q == 'PC':
            print(f"x{pc:04x}")
        elif q. startswith('x'):
            addr = int(q[1:], 16)
            print(f"x{memory[addr]:04x}")

if __name__ == '__main__':
    main()