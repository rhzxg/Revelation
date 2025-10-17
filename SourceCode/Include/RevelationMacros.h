#pragma once

#define REV_DECLARE_ENUM_OPERATOR(EnumType)                                          \
    inline EnumType operator|(EnumType lhs, EnumType rhs)                            \
    {                                                                                \
        return static_cast<EnumType>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
    }                                                                                \
    inline EnumType operator&(EnumType lhs, EnumType rhs)                            \
    {                                                                                \
        return static_cast<EnumType>(static_cast<int>(lhs) & static_cast<int>(rhs)); \
    }                                                                                \
    inline EnumType operator^(EnumType lhs, EnumType rhs)                            \
    {                                                                                \
        return static_cast<EnumType>(static_cast<int>(lhs) ^ static_cast<int>(rhs)); \
    }                                                                                \
    inline EnumType operator~(EnumType rhs)                                          \
    {                                                                                \
        return static_cast<EnumType>(~static_cast<int>(rhs));                        \
    }
