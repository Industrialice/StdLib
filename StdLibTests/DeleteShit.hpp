#ifndef __DELETE_SHIT_HPP__
#define __DELETE_SHIT_HPP__

class DeleteShit
{
    DeleteShit();

public:
    static DeleteShit *Create();
    void Destroy();
    void Perform();

    struct Deleter
    {
        Deleter( DeleteShit *ob )
        {
            ob->Destroy();
        }
    };
};

#endif