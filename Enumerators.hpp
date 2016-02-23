#ifndef ENUMERATORS
#define ENUMERATORS

// would have been  if(day == Mon){return "Mon";}
// instead of being RETURN_IF_ENUM_SET(Mon);
#define RETURN_IF_ENUM_SET(e, x) if(e == x) {return #x;}
#define RETURN_IF_ENUM_FOUND(str, x) if(str.find(#x) != std::string::npos) {return x;}


enum DAYNAMES{
    UnkD=0,Mon=1,Tue,Wed,
    Thu,Fri,Sat,Sun
};

enum MONTHNAMES{
    UnkM=0,Jan=1,Feb,Mar,Apr,May,
    Jun,Jul,Aug,Sep,Oct,Nov,Dec
};


DAYNAMES find_day(string date_from_line){
    RETURN_IF_ENUM_FOUND(date_from_line, Mon);
    RETURN_IF_ENUM_FOUND(date_from_line, Tue);
    RETURN_IF_ENUM_FOUND(date_from_line, Wed);
    RETURN_IF_ENUM_FOUND(date_from_line, Thu);
    RETURN_IF_ENUM_FOUND(date_from_line, Fri);
    RETURN_IF_ENUM_FOUND(date_from_line, Sat);
    RETURN_IF_ENUM_FOUND(date_from_line, Sun);
    return UnkD;
}
string day_to_string(DAYNAMES day){
    RETURN_IF_ENUM_SET(day, Mon);
    RETURN_IF_ENUM_SET(day, Tue);
    RETURN_IF_ENUM_SET(day, Wed);
    RETURN_IF_ENUM_SET(day, Thu);
    RETURN_IF_ENUM_SET(day, Fri);
    RETURN_IF_ENUM_SET(day, Sat);
    RETURN_IF_ENUM_SET(day, Sun);
    return "bad day";
}


MONTHNAMES find_month(string date_from_line){
    RETURN_IF_ENUM_FOUND(date_from_line, Jan);
    RETURN_IF_ENUM_FOUND(date_from_line, Feb);
    RETURN_IF_ENUM_FOUND(date_from_line, Mar);
    RETURN_IF_ENUM_FOUND(date_from_line, Apr);
    RETURN_IF_ENUM_FOUND(date_from_line, May);
    RETURN_IF_ENUM_FOUND(date_from_line, Jun);
    RETURN_IF_ENUM_FOUND(date_from_line, Jul);
    RETURN_IF_ENUM_FOUND(date_from_line, Aug);
    RETURN_IF_ENUM_FOUND(date_from_line, Sep);
    RETURN_IF_ENUM_FOUND(date_from_line, Oct);
    RETURN_IF_ENUM_FOUND(date_from_line, Nov);
    RETURN_IF_ENUM_FOUND(date_from_line, Dec);
    return UnkM;
}
string month_to_string(MONTHNAMES month){
    RETURN_IF_ENUM_SET(month, Jan);
    RETURN_IF_ENUM_SET(month, Feb);
    RETURN_IF_ENUM_SET(month, Mar);
    RETURN_IF_ENUM_SET(month, Apr);
    RETURN_IF_ENUM_SET(month, May);
    RETURN_IF_ENUM_SET(month, Jun);
    RETURN_IF_ENUM_SET(month, Jul);
    RETURN_IF_ENUM_SET(month, Aug);
    RETURN_IF_ENUM_SET(month, Sep);
    RETURN_IF_ENUM_SET(month, Oct);
    RETURN_IF_ENUM_SET(month, Nov);
    RETURN_IF_ENUM_SET(month, Dec);
    return "bad month";
}

// fragile macros... do not let use outside of file
#undef RETURN_IF_ENUM_SET
#undef RETURN_IF_ENUM_FOUND


#endif // ENUMERATORS
