# @Author: Nick Steele
# @Date:   9:08 Oct 07 2021
# @Last modified by:   Nick Steele
# @Last modified time: 9:23 Oct 07 2021

# Generates a few lines of the tabular code

datatypes = [
    'float',
    'uint8_t',
    'uint16_t',
    'uint32_t',
    'int8_t',
    'int16_t',
    'int32_t',
]

max_datatype_length = max([len(x) for x in datatypes])


def printSpaces(fmt):
    return ' ' * (max_datatype_length - len(fmt) + 1)


print('// Generated using gen-tabular.py')
print('enum _TabularDataFmt_enum {')
[print('  FMT_' + fmt.upper() + ',') for fmt in datatypes]
print('};\n')
print('typedef struct TabularData_t {\n  union {')
[print('' + fmt + printSpaces(fmt) + '*' + fmt + '_ptr;') for fmt in datatypes]
print('} ptr;')
print('''  enum _TabularDataFmt_enum fmt;
} TabularDataPtr_t;''')

print('\n\n\n\n')
print('// Generated using gen-tabular.py')
print('switch(tabular_ptr->fmt) {')
for fmt in datatypes:
    print(f'''case FMT_{fmt.upper()}:
    LOG_DATA(*tabular_ptr->ptr.{fmt + '_ptr'});
    break;''')
print('} // switch(tabular_ptr->fmt)')
