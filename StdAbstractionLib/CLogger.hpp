#ifndef __CLOGGER_HPP__
#define __CLOGGER_HPP__

namespace StdLib {

class CLogger
{
protected:
    CLogger() = default;
    CLogger( const CLogger & ) = delete;
    CLogger &operator =( const CLogger & ) = delete;

public:
    enum class Tag 
	{
        info = 0,
        warning = 1,
        error = 2,
        debug = 3,
        user = 4,
        important = 5,
		critical = 6 
	};

    typedef void (*DirectionFunc)( Tag tag, const char *cp_text, uiw len );
    
#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
    void _Message( Tag tag, const char *cp_fmt, ... );

    template < typename... Args > void Message( Tag tag, const char *cp_fmt, const Args &... args )
    {
        if( !Funcs::_AreArgsValid( cp_fmt, args... ) )
        {
            SOFTBREAK;
            return;
        }
        _Message( tag, cp_fmt, args... );
    }
#else
    void Message( Tag tag, const char *cp_fmt, ... );
#endif
    void DirectionAdd( DirectionFunc dir );
    uiw DirectionsCount() const;
    DirectionFunc DirectionGet( uiw index ) const;
	void DirectionRemove( uiw index );
	void DirectionToTheTop( uiw index );
    void IsOnSet( bool is_on );
    bool IsOnGet() const;
    bool IsOnToggle();  //  returns new state
    bool IsMultithreadedGet() const;
    void IsMitlithreadedSet( bool is_multithreaded );
    const char *NameGet() const;
    static WARN_IF_UNUSED CLogger *Create( const char *cp_name, bool is_on, bool is_multithreaded );  //  isn't thread safe
    static void Delete( CLogger *po_logger );
};

}  //  namespace StdLib

#endif
