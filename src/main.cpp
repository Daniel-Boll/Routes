#include "DLinkedList.hpp"
/* #include "json.hpp" */

#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <locale>

#include <codecvt>
#include <vector>
#include <map>
#include <memory>

#include "ftxui/component/container.hpp"
#include "ftxui/component/button.hpp"
#include "ftxui/component/checkbox.hpp"
#include "ftxui/component/input.hpp"
#include "ftxui/component/menu.hpp"
#include "ftxui/component/radiobox.hpp"
#include "ftxui/component/toggle.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "ftxui/screen/string.hpp"

using namespace ftxui;

class About : public Component {
  Element Render() override {
    return hbox(text(L"About") | center) | flex | border;
  }
};

class RoutesData : public DLinkedList<std::vector<std::wstring>> {
  public:
    std::map<
      std::wstring,
      DLinkedList<std::vector<std::wstring>>
    > routesData;

    node *current; 
    std::wstring currentName;
    int8_t currentIdx;

    RoutesData() {
      currentIdx = -1;

      // For the sake of time I will hard coded the default entries
      // TODO: Get from JSON file

      DLinkedList<std::vector<std::wstring>> t;
      t.add(std::vector<std::wstring>{L"Curitiba", L"Mas aqui tá frio, tá ligado?!"});
      t.add(std::vector<std::wstring>{L"Ponta Grossa", L"Lagoa Dourada"});
      t.add(std::vector<std::wstring>{L"Guarapuava", L"Nunca fui, não faço ideia"});
      t.add(std::vector<std::wstring>{L"Cascavel", L"Cidade de capivaras"});
      routesData.insert(std::make_pair(L"Cascavel", t));

      DLinkedList<std::vector<std::wstring>> t2;
      t2.add(std::vector<std::wstring>{L"Curitiba", L"Mas aqui tá frio, tá ligado?!"});
      t2.add(std::vector<std::wstring>{L"Ponta Grossa", L"Lagoa Dourada"});
      t2.add(std::vector<std::wstring>{L"Telêmaco Borba", L"Nome de respeito"});
      t2.add(std::vector<std::wstring>{L"Londrina", L"Dá-lhe gente tóxica"});
      routesData.insert(std::make_pair(L"Londrina", t2));

      current = routesData.begin()->second.getSentinel().getFirst();
      changeRoute(0);
    }

    void changeRoute(int8_t flag) {
      // ============= Constraints ============
      !flag ? currentIdx++ : currentIdx--;
      if(currentIdx < 0) currentIdx = 0;
      if(currentIdx > routesData.size() - 1) currentIdx = routesData.size() - 1;
      // ======================================

      int8_t idx = 0;
      
      for(auto it = routesData.begin(); it != routesData.end(); it++) {
        if(idx == currentIdx) {
          currentName = it->first;
          current = it->second.getSentinel().getFirst();
          break;
        }
        idx++;
      }
    }

    std::wstring currentRouteName() {
      return currentName; 
    }

    std::wstring currentCityName() {
       return current->getData().front(); 
    }
   
    std::wstring currentCityBio() {
      return current->getData().back();
    }

    void nextCity() {
      if(current->getNext() != nullptr) current = current->getNext();
    }

    void prevCity() {
      if(current->getPrev() != nullptr) current = current->getPrev();
    }

    std::map<
      std::wstring, 
      DLinkedList<std::vector<std::wstring>>
    >* getRoutes() {
        return &this->routesData;
      }

    void setRoutes() {
      // TODO: Add new entries to a key from the map.
    }
};

class Checkout : public Component {
  Container container_ = Container::Vertical();

  public:
    std::shared_ptr<RoutesData> routesDataRef; 
    
    std::wstring routeName, cityName, cityBio;
    std::wstring keyPressed;
    std::wstring keyPressedDebug;

    int8_t idx;

    Checkout() {
      Add(&container_);
      idx = 0;
    }

    Element Render() override {
      routeName = routesDataRef.get()->currentRouteName();
      cityName = routesDataRef.get()->currentCityName();
      cityBio = routesDataRef.get()->currentCityBio();

      auto currentRouteName = text(routeName) | center;
      auto cityInfo = vbox({
        text(cityName) | center,
        separator(),
        text(cityBio)
      });
      
      auto key = text(
        keyPressedDebug.empty() 
          ? L"Key to be pressed"
          : keyPressedDebug
      ) | center;
      
      auto footer = vbox({
        // ------------------------------
        // Filler not working, dunno why
        text(L""),
        text(L""),
        text(L""),
        text(L""),
        text(L""),
        text(L""),
        text(L""),
        // ------------------------------
        text(L"City navigation: -> | <-") | center,
        text(L"Change route: PgUp | PgDown") | center
      });

      return vbox({vbox({ 
          currentRouteName,
          separator(),
          vbox({
              cityInfo,
              separator(),
              key
          }) | center | border,
          }) ,
          filler(), 
          footer
        })| center | border;
    }

  bool OnEvent(Event event) override {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    keyPressed = converter.from_bytes(event.input()); 
   
    std::wstring leftTemplate = converter.from_bytes(event.ArrowLeft.input());
    std::wstring rightTemplate = converter.from_bytes(event.ArrowRight.input());
    std::wstring pageUpTemplate = converter.from_bytes(event.PageUp.input());
    std::wstring pageDownTemplate = converter.from_bytes(event.PageDown.input());
    

    // Should actually change here to boost::iequal
    if(isequal(keyPressed, leftTemplate)) {
      keyPressedDebug = L"<-";
      routesDataRef.get()->prevCity();
    } else if (isequal(keyPressed, rightTemplate)) {
      keyPressedDebug = L"->";
      routesDataRef.get()->nextCity();
    } else if (isequal(keyPressed, pageUpTemplate)) {
      routesDataRef.get()->changeRoute(0);
    } else if (isequal(keyPressed, pageDownTemplate)) {
      routesDataRef.get()->changeRoute(1);
    }

    return true;
  }

  bool isequal(const std::wstring& first, const std::wstring& second) {
      if(first.size() != second.size())
          return false;

      for(std::wstring::size_type i = 0; i < first.size(); i++)
      {
          if(first[i] != second[i] && first[i] != (second[i] ^ 32))
              return false;
      }

      return true;
  }
};

class RoutesAddModal : public Component {
  private:
    Container container_ = Container::Vertical();
    
    // ------------------ LEFT SIDE ---------------------
    Container routeInputContainer = Container::Vertical();
    Container cityNameInputContainer = Container::Vertical();
    Container cityBioInputContainer = Container::Vertical();
    Container addCityButtonContainer = Container::Vertical();

    ftxui::Input routeInput, cityNameInput, cityBioInput;
    ftxui::Button addCity = Button(L"Add");
    // ---------------------------------------------------


    // ------------------ RIGHT SIDE ---------------------
    Container cityListContainer = Container::Vertical();
    Menu cityListMenu;
    // ---------------------------------------------------
  public:
    std::wstring routeNameKey;

    // TODO: Should grab from the current cityKeys
    // actually fairly easy, but I got no time
    std::vector<std::wstring> cityListVec {
        L"Cascavel",
        L"Toledo",
        L"Paranaguá",
        L"Campinas",
        L"Curitiba"
    };
    
    // TODO: From the specific cityKey grab it's cities.
    std::vector<std::wstring> cityBioVec {
      L"Cascavel é uma cidade turística devida às suas capivaras",
      L"Cascavel wannabe",
      L"Paranaguá? Nem sei que cidade é essa na real",
      L"Campinas? WTF isso é no Paraná?",
      L"Curitiba pô, tá frio tá ligado? kkk frio demais uiui"
    };

    RoutesAddModal() {
      Add(&container_);
      
      // ======================= LEFT SIDE ============================
      // ---------- INPUT -----------
      container_.Add(&routeInputContainer);
      container_.Add(&cityNameInputContainer);
      container_.Add(&cityBioInputContainer);

      routeInput.placeholder = L"Input route name";
      routeInput.on_enter = [this] {
        // Use this variable to search bla bla bla
        routeNameKey = routeInput.content;
        /* if(RoutesData::routes.find(routeNameKey)->first == routeNameKey) routeNameKey = L"Achei"; */
      };

      cityNameInput.placeholder = L"City name to be added";
      cityNameInput.on_enter = [this] {
        // Add to the DLinkedList on route key
      };

      cityBioInput.placeholder = L"City description to be added";
      cityBioInput.on_enter = [this] {
        // Add to the DLinkedList on route key
      };

      routeInputContainer.Add(&routeInput);
      cityNameInputContainer.Add(&cityNameInput);
      cityBioInputContainer.Add(&cityBioInput);

      container_.Add(&addCityButtonContainer);
      addCityButtonContainer.Add(&addCity);
      // ==============================================================

  
      // ======================= RIGHT SIDE ===========================
      container_.Add(&cityListContainer);
      cityListMenu.entries = cityListVec; 
      cityListMenu.normal_style = dim | color(Color::YellowLight);
      cityListMenu.selected_style = color(Color::YellowLight);
      cityListMenu.focused_style = bold | color(Color::YellowLight);

      cityListContainer.Add(&cityListMenu);
      // ==============================================================
    }

    Element Render() override {
      auto routeName = window(text(L"Route"), routeInput.Render());
      auto cityName = window(text(L"Add city"), cityNameInput.Render());
      auto cityBio = window(text(L"Add city bio"), cityBioInput.Render());

      auto cityList = window(text(L"City List"), cityListMenu.Render() | frame);
      return hbox({ 
        vbox({
          routeName,
          separator(),
          vbox({
            cityName,
            cityBio,
            addCity.Render() | size(WIDTH, EQUAL, 10) | center,
          })
        }) | flex | size(WIDTH, GREATER_THAN, 40),
        separator(),
        vbox({
          (routeNameKey.empty())
            ? text(L"Insert the city name that you want to consult!") | center
            :
              vbox({
                text(routeNameKey) | center,
                separator(),
                hbox({
                  cityList | size(HEIGHT, LESS_THAN, 10),
                  separator(),
                  text(cityBioVec.at(cityListMenu.selected)) 
                })
              })
        }) | flex | size(WIDTH, GREATER_THAN, 40)
      }) | border;
    }
};


class Routes : public Component {

  // -------------- Containers ---------------
  Container container = Container::Vertical();

  Container addAndLoad = Container::Horizontal();
  Container save = Container::Horizontal();
  // -----------------------------------------

  // -------------- Modals -------------------
  RoutesAddModal routesAddModal_;
  // -----------------------------------------

  // ----------------- Buttons ------------------
  ftxui::Button addButton, loadButton, saveButton;
  
  std::function<void()> onAdd = [&] { routesAddModal_.TakeFocus(); };
  std::function<void()> onSave = [&] { onQuit(); };
  // --------------------------------------------
  
  std::wstring xapa;

  public:
    std::function<void()> onQuit = [] {};
    std::shared_ptr<RoutesData> routesDataRef; 

    Routes() {
      Add(&container);

      addButton.label = L"Add Route";
      addButton.on_click = [&] { onAdd(); };
      
      loadButton.label = L"Load Routes";
      loadButton.on_click = [&] {
        xapa = routesDataRef.get()->routesData.begin()->first;

        routesDataRef.get()->setRoutes();
      };

      saveButton.label = L"Save and quit";
      /* saveButton.on_click = [&] {  }; */

      addAndLoad.Add(&addButton);
      addAndLoad.Add(&loadButton);
      save.Add(&saveButton);

      container.Add(&addAndLoad);
      container.Add(&save);
      container.Add(&routesAddModal_);
    }

  Element Render() override {
    auto title = hbox({
      text((xapa.empty()) ? L"ROUTE MANAGEMENT" : xapa) | bold | center | color(Color::YellowLight)
    });

    auto buttons = vbox({
      hbox(addAndLoad.Render()),
      hbox(save.Render()) | center
    }) | center;

    auto footer = hbox({
      text(L"© All rights reserver for Daniel™") | center
    });

    auto content = vbox(
        title,
        filler(),
        buttons,
        filler(),
        footer
    ) | flex | border; 

    Element document = content;
    if(routesAddModal_.Focused()) {
      document = dbox({
        document,
        routesAddModal_.Render() | clear_under | center
      });
    }

    return document;
  }
};

class Home : public Component {
  Element Render() override {
    auto title = hbox({
      text(L"Welcome to Daniel's "),
      text(L"Bus System ") | blink | bold | color(Color::YellowLight),
      text(L"Management") 
    }) | center;

    auto footer = hbox({
      text(L"© All rights reserver for Daniel™") | center
    });

    return
      vbox({
        filler(),
        title,
        filler(),
        footer
      }) | flex | border; 
  }
};

class Tab : public Component {
  public:
    Container mainContainer = Container::Vertical();
    
    std::function<void()> onQuit = [] {};

    Toggle tabSelection;
    Container container = Container::Tab(&tabSelection.selected);

    Home home;
    Routes routes;
    Checkout checkout;
    About about;

    Tab() {
      auto shared_data = std::make_shared<RoutesData>();

      Add(&mainContainer);
      mainContainer.Add(&tabSelection);
      tabSelection.entries = {
        L"Home", L"Routes", L"Checkout", L"About",
      };
      mainContainer.Add(&container);
      
      routes.routesDataRef = shared_data;
      checkout.routesDataRef = shared_data;

      container.Add(&home);
      container.Add(&routes);
      container.Add(&checkout);
      container.Add(&about);
    }

    Element Render() override {
      return vbox({
        text(L"Daniel's Bus System") | bold | hcenter,
        tabSelection.Render() | hcenter,
        container.Render() | flex,
      });
    }
};

int shift = 0;

int main() {
  auto screen = ScreenInteractive::Fullscreen();
  
  std::thread update([&screen]() {
    for(;;) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      shift++;
      screen.PostEvent(Event::Custom);
    }
  });

  Tab tab;
  tab.onQuit = screen.ExitLoopClosure();
  screen.Loop(&tab);

  return 0;
}
