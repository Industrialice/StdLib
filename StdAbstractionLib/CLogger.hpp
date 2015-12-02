#ifndef __CLOGGER_HPP__
#define __CLOGGER_HPP__

namespace StdLib {

class CLogger
{
protected:
    CLogger();
    CLogger( const CLogger & );
    CLogger &operator =( const CLogger & );

public:
    struct Tag
    {
        CONSTS( messageTag_t,
                info = 0,
                warning = 1,
                error = 2,
                debug = 3,
                user = 4,
                important = 5 )
    };

    typedef void (*DirectionFunc)( Tag::messageTag_t tag, const char *cp_text, size_t len );

    va_return Message( Tag::messageTag_t tag, const char *cp_fmt, ... );
    void AddDirection( DirectionFunc dir );
    void PopDirection();
    size_t DirectionsCount() const;
    DirectionFunc DirectionByIndexGet( size_t index ) const;
    void DirectionByIndexSet( size_t index, DirectionFunc dir );
    void BufferLengthSet( unsigned int length );
    unsigned int BufferLengthGet() const;
    void IsOnSet( bool is_on );
    bool IsOnGet() const;
    bool IsOnToggle();  //  return new state
    bool IsMultithreadedGet() const;
    void IsMitlithreadedSet( bool is_multithreaded );
    const char *NameGet() const;
    static CLogger *Create( const char *cp_name, bool is_on, bool is_multithreaded, unsigned int bufferSize = 2048 );
    static void Delete( CLogger *po_logger );
};

}  //  namespace StdLib

#endif __CLOGGER_HPP__
