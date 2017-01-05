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
                important = 5,
				critical = 6 )
    };

    typedef void (*DirectionFunc)( Tag::messageTag_t tag, const char *cp_text, uiw len );
    
#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
    void _Message( Tag::messageTag_t tag, const char *cp_fmt, ... );

    template < typename... Args > void Message( Tag::messageTag_t tag, const char *cp_fmt, const Args &... args )
    {
        if( !Funcs::_AreArgsValid( cp_fmt, args... ) )
        {
            SOFTBREAK;
            return;
        }
        _Message( tag, cp_fmt, args... );
    }
#else
    void Message( Tag::messageTag_t tag, const char *cp_fmt, ... );
#endif
    void DirectionAdd( DirectionFunc dir );
    uiw DirectionsCount() const;
    DirectionFunc DirectionGet( uiw index ) const;
	void DirectionRemove( uiw index );
	void DirectionToTheTop( uiw index );
    void IsOnSet( bln is_on );
    bln IsOnGet() const;
    bln IsOnToggle();  //  returns new state
    bln IsMultithreadedGet() const;
    void IsMitlithreadedSet( bln is_multithreaded );
    const char *NameGet() const;
    static CLogger *Create( const char *cp_name, bln is_on, bln is_multithreaded );  //  isn't thread safe
    static void Delete( CLogger *po_logger );
};

}  //  namespace StdLib

#endif
