#include "Engine.hpp"
#include "LoadingScreen.hpp"
#include "MainMenu.hpp"
//#include "CreditsScreen.hpp"
#include "GameScene.hpp"
#include <memory>

#include "globals.hpp"
#if BUILD_LEVEL_EDITOR
    #include "LevelEditor.hpp"
#endif


namespace {
    Engine engine;
    const string assets_dir;
}


int main(int, char**)
{
    if(engine.init()) return -1;


    engine.set_scene(EngineMode::loading, std::make_shared
                     <LoadingScreen>(&engine, "assets/"));
    engine.update_screen();
    engine.load("assets/");

#if BUILD_LEVEL_EDITOR
    engine.set_scene(EngineMode::map_editor,
                     std::make_shared<LevelEditor>
                     (&engine));
#endif

    engine.set_scene(EngineMode::menu, std::make_shared
                     <MainMenu>(&engine));

    /*
    engine.set_scene(EngineMode::credits, std::make_shared
                     <CreditsScreen>(&engine));
    */
    engine.set_scene(EngineMode::playing, std::make_shared
                     <GameScene>(&engine, "main"));

    engine.run();
    


    return 0;
}
