#include "view/MainWindow.h"
#include "model/Crossword.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

void usage(){
    std::cout << "Usage ./CrossedWord term [easy/normal/hard/test/test_import_export] -p 'pattern_path' -bd 'bd_path'" << std::endl;
    exit(1);
}

int launcherTerm(int argc, char *argv[]){
    if(argc != 2)
        if(argc !=4 && argc != 6) usage(); 

    try{
        Lexicon* lexicon;
        if(argv[1] != "test")
            lexicon = Lexicon::getInstance("../../data/databases/word_database.txt"); //constructs the singleton object representing the database used
        if(!strcmp(argv[1],"easy")){
            if(argc == 2) {
                Crossword* game = new Crossword(EASY);
                printf("ok %d \n", 1);
            }
                else
                    if(!strcmp(argv[2],"-p"))
                        Crossword* game = new Crossword(EASY,argv[3]);
                    else
                        usage();
        }
        else if(!strcmp(argv[1],"normal")){
            if(argc == 2){
                Crossword* game = new Crossword(NORMAL);
                printf("ok %d \n", 1);
            }

                else
                    if(!strcmp(argv[2],"-p"))
                        Crossword* game = new Crossword(NORMAL,argv[3]);
                    else
                        usage();
        }
        else if(!strcmp(argv[1],"hard")){
            if(argc == 2)
                Crossword* game = new Crossword(HARD);
                else
                    if(!strcmp(argv[2],"-p"))
                        Crossword* game = new Crossword(HARD,argv[3]);
                    else
                        usage();
        }
        else if(!strcmp(argv[1],"test")){
            if(argc == 6){
                lexicon = Lexicon::getInstance(argv[5]);
                if(!strcmp(argv[2],"-p")){
                    Crossword* game = new Crossword(TEST,argv[3]);
                    std::vector<Word> words = game->getAllWords();
                    for (unsigned i=0; i<words.size(); i++){
                        std::string orientation;
                        words.at(i).horizontal ? orientation = "horizontal" : orientation = "vertical";
                        std::cout << '(' << words.at(i).number << "," << words.at(i).word << " ["<< words.at(i).pos.x << "," << words.at(i).pos.y << "], " << orientation << ")" << std::endl;
                    }
                    std::cout << '\n';
                }
                else
                    usage();
            }
            else
                usage();    
        }
        else if(!strcmp(argv[1],"test_import_export")){
            if(argc > 4){
                if(!strcmp(argv[2],"-p")){
                    Crossword* game = new Crossword(argv[3]);
                    game->exportGame("../../data/user_saves/export_test.json");
                }
                else
                    usage();
            }
            else
                usage();  
        }
        else{
            usage();
        }
        lexicon->releaseInstance();
    }catch(CrosswordGenerationException e){
        std::cerr << e.what() << std::endl;
    }catch(CrosswordBadAccessException e){
        std::cerr << e.what() << std::endl;
    }catch(std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

/**
 * @brief 
 * CMD: mode terminal : ./CrossedWord term test -p ../../data/patterns/easy/easy2.txt -bd ../../data/word_database.txt (exemple de commdande)
   CMD: mode graphique : ./CrossedWord
 */
int main(int argc, char *argv[])
{
    if(argc != 1 && !strcmp(argv[1], "term"))
        return launcherTerm(argc-1,argv+1);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MotsCroisesSrcTmp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
