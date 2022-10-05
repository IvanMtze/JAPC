//
// Created by wuser on 8/09/22.
//

#ifndef JAPC_CHARACTERDEFINITIONS_H
#define JAPC_CHARACTERDEFINITIONS_H

#define NULL_CHARACTER 0
#define MAX_ASCII_CHARACTER 0x7F
#define LINEFEED 0x0A        // \n
#define CARRIAGE_RETURN 0x0D // \r
#define LINE_SEPARATOR 0x2028
#define PARAGRAPH_SEPARATOR 0x2029
#define NEXT_LINE                                                                                                      \
    0x0085 // \
/* Unicode 3.0 space characters*/
#define SPACE 0x0020
#define NON_BREAKING_SPACE 0x00A0
#define EN_QUAD 0x2000
#define EM_QUAD 0x2001
#define EN_SPACE 0x2002
#define EM_SPACE 0x2003
#define THREE_PER_EM_SPACE 0x2004
#define FOUR_PER_EM_SPACE 0x2005
#define SIX_PER_EM_SPACE 0x2006
#define FIGURE_SPACE 0x2007
#define PUNCTUATION_SPACE 0x2008
#define THIN_SPACE 0x2009
#define HAIR_SPACE 0x200A
#define ZERO_WIDTH_SPACE 0x200B
#define NARROW_NO_BREAK_SPACE 0x202F
#define IDEOGRAPHIC_SPACE 0x3000
#define MATHEMATICAL_SPACE 0x205F
#define OGHAM 0x1680
#define LITERAL__ 0x5F
#define LITERAL_$ 0x24

#define LITERAL_0 0x30
#define LITERAL_1 0x31
#define LITERAL_2 0x32
#define LITERAL_3 0x33
#define LITERAL_4 0x34
#define LITERAL_5 0x35
#define LITERAL_6 0x36
#define LITERAL_7 0x37
#define LITERAL_8 0x38
#define LITERAL_9 0x39
#define LITERAL_a 0x61
#define LITERAL_b 0x62
#define LITERAL_c 0x63
#define LITERAL_d 0x64
#define LITERAL_e 0x65
#define LITERAL_f 0x66
#define LITERAL_g 0x67
#define LITERAL_h 0x68
#define LITERAL_i 0x69
#define LITERAL_j 0x6A
#define LITERAL_k 0x6B
#define LITERAL_l 0x6C
#define LITERAL_m 0x6D
#define LITERAL_n 0x6E
#define LITERAL_o 0x6F
#define LITERAL_p 0x70
#define LITERAL_q 0x71
#define LITERAL_r 0x72
#define LITERAL_s 0x73
#define LITERAL_t 0x74
#define LITERAL_u 0x75
#define LITERAL_v 0x76
#define LITERAL_w 0x77
#define LITERAL_x 0x78
#define LITERAL_y 0x79
#define LITERAL_z 0x7A

#define LITERAL_A 0x41
#define LITERAL_B 0x42
#define LITERAL_C 0x43
#define LITERAL_D 0x44
#define LITERAL_E 0x45
#define LITERAL_F 0x46
#define LITERAL_G 0x47
#define LITERAL_H 0x48
#define LITERAL_I 0x49
#define LITERAL_J 0x4A
#define LITERAL_K 0x4B
#define LITERAL_L 0x4C
#define LITERAL_M 0x4D
#define LITERAL_N 0x4E
#define LITERAL_O 0x4F
#define LITERAL_P 0x50
#define LITERAL_Q 0x51
#define LITERAL_R 0x52
#define LITERAL_S 0x53
#define LITERAL_T 0x54
#define LITERAL_U 0x55
#define LITERAL_V 0x56
#define LITERAL_W 0x57
#define LITERAL_X 0x58
#define LITERAL_Y 0x59
#define LITERAL_Z 0x5a

#define AMPERSAND 0x26     // &
#define ASTERISK 0x2A      // *
#define AT 0x40            // @
#define BACKSLASH 0x5C     /* \ */
#define BACKTICK 0x60      // `
#define BAR 0x7C           // |
#define CARET 0x5E         //
#define CLOSE_BRACE 0x7D   // }
#define CLOSE_BRACKET 0x5D // ]
#define CLOSE_PAREN 0x29   // )
#define COLON 0x3A         // :
#define COMMA 0x2C         // ,
#define DOT 0x2E           // .
#define DOUBLE_QUOTE 0x22  // "
#define EQUALS 0x3D        // =
#define EXCLAMATION 0x21   // !
#define GREATER_THAN 0x3E  // >
#define HASH 0x23          // #
#define LESS_THAN 0x3C     // <
#define MINUS 0x2D         // -
#define OPEN_BRACE 0x7B    // {
#define OPEN_BRACKET 0x5B  // [
#define OPEN_PAREN 0x28    // (
#define PERCENT 0x25       // %
#define PLUS 0x2B          // +
#define QUESTION 0x3F      // ?
#define SEMICOLON 0x3B     // ;
#define SINGLE_QUOTE 0x27  // '
#define SLASH 0x2F         // /
#define TILDE 0x7E         // ~

#define BACKSPACE 0x08 // \b
#define FORM_FEED 0x0C // \f
#define BYTE_ORDER_MARK 0xFEFF
#define TAB 0x09          // \t
#define VERTICAL_TAB 0x0B // \v
#endif                    // JAPC_CHARACTERDEFINITIONS_H
