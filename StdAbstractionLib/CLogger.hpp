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

    typedef void (*DirectionFunc)( Tag::messageTag_t tag, const char *cp_text, uiw len );

    va_return Message( Tag::messageTag_t tag, const char *cp_fmt, ... );
    void AddDirection( DirectionFunc dir );
    void PopDirection();
    uiw DirectionsCount() const;
    DirectionFunc DirectionByIndexGet( uiw index ) const;
    void DirectionByIndexSet( uiw index, DirectionFunc dir );
    void BufferLengthSet( ui32 length );
    ui32 BufferLengthGet() const;
    void IsOnSet( bln is_on );
    bln IsOnGet() const;
    bln IsOnToggle();  //  return new state
	bln IsMultithreadedGet() const;
	void IsMitlithreadedSet( bln is_multithreaded );
    const char *NameGet() const;
    static CLogger *Create( const char *cp_name, bln is_on, bln is_multithreaded, ui32 bufferSize = 2048 );
    static void Delete( CLogger *po_logger );
};

}  //  namespace StdLib

#endif __CLOGGER_HPP__
