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
    
#if defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
    void _Message( Tag::messageTag_t tag, const char *cp_fmt, ... );

    template < typename... Args > void Message( Tag::messageTag_t tag, const char *cp_fmt, const Args &... args )
    {
        if( !Funcs::_AreArgsValid( cp_fmt, args... ) )
        {
            DBGBREAK;
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
    bool IsOnGet() const;
    bool IsOnToggle();  //  returns new state
    bool IsMultithreadedGet() const;
    void IsMitlithreadedSet( bool is_multithreaded );
    const char *NameGet() const;
    static CLogger *Create( const char *cp_name, bool is_on, bool is_multithreaded );
    static void Delete( CLogger *po_logger );
};

}  //  namespace StdLib

#endif
