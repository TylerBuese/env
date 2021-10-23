struct Irc 
{
    int level1(int selectedMessage)
    {
        switch (selectedMessage)
        {
            case 0: return 2004; break;
            case 1: return 2008; break;
            case 2: return 2012; break;
        }
    }

    int level2(int selectedMessage)
    {
        switch (selectedMessage)
        {
            case 0: return 2016; break;
            case 1: return 2020; break;
            case 2: return 2024; break;
        }
    }
};