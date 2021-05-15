#include "DLinkedList.hpp"
/* #include "json.hpp" */

#include <chrono>
#include <iostream>
#include <locale>
#include <string>

#include <codecvt>
#include <vector>
#include <map>
#include <memory>

#include "ftxui/component/component.hpp"
#include "ftxui/component/menu.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "ftxui/screen/string.hpp"

using namespace ftxui;

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
      t.add({L"Curitiba", L"Mas aqui tá frio, tá ligado?!"});
      t.add({L"Ponta Grossa", L"Lagoa Dourada"});
      t.add({L"Guarapuava", L"Nunca fui, não faço ideia"});
      t.add({L"Cascavel", L"Cidade de capivaras"});
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

class Checkout : public ComponentBase {
  Component container_ = Container::Vertical({});

  public:
    std::shared_ptr<RoutesData> routesDataRef; 
    
    std::wstring routeName, cityName, cityBio;
    std::wstring keyPressed;
    std::wstring keyPressedDebug;

    int8_t idx;

    Checkout() {
      Add(container_);
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
          text(cityBio),
      });

      auto key = text(keyPressedDebug.empty() ? L"Key to be pressed"
                                              : keyPressedDebug) |
                 center;
      return vbox({
                 currentRouteName,
                 separator(),
                 vbox({cityInfo, separator(), key}) | hcenter | border,
                 filler(),
                 text(L"City navigation: -> | <-") | hcenter,
                 text(L"Change route: PgUp | PgDown") | hcenter,
                 filler(),
             }) |
             center | border;
    }

  bool OnEvent(Event event) override {
    // Should actually change here to boost::iequal
    if(event == Event::ArrowLeft) {
      keyPressedDebug = L"<-";
      routesDataRef.get()->prevCity();
      return true;
    }

    if (event == Event::ArrowRight) {
      keyPressedDebug = L"->";
      routesDataRef.get()->nextCity();
      return true;
    }

    if (event == Event::PageUp) {
      routesDataRef.get()->changeRoute(0);
      return true;
    }

    if (event == Event::PageDown) {
      routesDataRef.get()->changeRoute(1);
      return true;
    }

    return false;
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

class RoutesAddModal : public ComponentBase {
  private:
    // ------------------ LEFT SIDE ---------------------
    std::wstring routeNameKey;
    std::wstring cityName;
    std::wstring cityBio;
    Component routeInput = Input(&routeNameKey, "Input route name");
    Component cityNameInput = Input(&cityName, "City name to be added");
    Component cityBioInput = Input(&cityBio, "City description to be added");

    Component addCity = Button(L"Add", [] {});

    // ------------------ RIGHT SIDE ---------------------
    Component cityListContainer = Container::Vertical({});

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
    int cityListSelected = 0;
    Component cityListMenu = Menu(&cityListVec, &cityListSelected);

    // ---- Main ---
    Component container_ = Container::Horizontal({
        Container::Vertical({
            routeInput,
            cityNameInput,
            cityBioInput,
            addCity,
        }),
        cityListMenu,
    });
    // ---------------------------------------------------
  public:

    RoutesAddModal() {
      Add(container_);

      MenuBase::From(cityListMenu)->normal_style =
          dim | color(Color::YellowLight);
      MenuBase::From(cityListMenu)->selected_style = color(Color::YellowLight);
      MenuBase::From(cityListMenu)->focused_style =
          bold | color(Color::YellowLight);
    }

    Element Render() override {
      auto routeName = window(text(L"Route"), routeInput->Render());
      auto cityName = window(text(L"Add city"), cityNameInput->Render());
      auto cityBio = window(text(L"Add city bio"), cityBioInput->Render());
      auto cityList = window(text(L"City List"), cityListMenu->Render() | frame);
      return hbox({ 
        vbox({
          routeName,
          separator(),
          vbox({
            cityName,
            cityBio,
            addCity->Render() | size(WIDTH, EQUAL, 10) | center,
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
                  text(cityBioVec.at(cityListSelected)),
                })
              })
        }) | flex | size(WIDTH, GREATER_THAN, 40)
      }) | border;
    }
};


class Routes : public ComponentBase {

  // -----------------------------------------

  // -------------- Modals -------------------
  int modal_shown = 0;
  Component routesAddModal_ = Make<RoutesAddModal>();
  // -----------------------------------------

  // ----------------- Buttons ------------------
  Component addButton = Button(L"Add Route", [this] { modal_shown = 1; });
  Component loadButton = Button(L"Load Routes", [this] {
    xapa = routesDataRef.get()->routesData.begin()->first;
    routesDataRef.get()->setRoutes();
  });
  Component saveButton = Button(L"Save and quit", [] {});

  // -------------- Containers ---------------

  Component main_container = Container::Tab(
      {
          Container::Vertical({
              Container::Horizontal({
                  addButton,
                  loadButton,
              }),
              saveButton,
          }),
          routesAddModal_,
      },
      &modal_shown);

  std::wstring xapa;

  public:
    std::function<void()> onQuit = [] {};
    std::shared_ptr<RoutesData> routesDataRef;

    Routes() { Add(main_container); }

    Element Render() override {
      auto title = text((xapa.empty()) ? L"ROUTE MANAGEMENT" : xapa) | bold |
                   hcenter | color(Color::YellowLight);

      auto buttons = vbox({
                         hbox({
                             addButton->Render(),
                             loadButton->Render(),
                         }),
                         saveButton->Render() | xflex,
                     }) |
                     center;

      auto footer = text(L"© All rights reserver for Daniel™") | hcenter;

      auto document = vbox({
                          title,
                          filler(),
                          buttons,
                          filler(),
                          footer,
                      }) |
                      flex | border;

      if (modal_shown) {
        document =
            dbox({document, routesAddModal_->Render() | clear_under | center});
      }

      return document;
    }
};

int main() {
  auto tab_home = Renderer([] {
    auto title =
        hbox({
            text(L"Welcome to Daniel's "),
            text(L"Bus System ") | blink | bold | color(Color::YellowLight),
            text(L"Management"),
        }) |
        hcenter;

    auto footer = text(L"© All rights reserver for Daniel™") | hcenter;

    return vbox({
               filler(),
               title,
               filler(),
               footer,
           }) |
           border;
  });

  std::shared_ptr<Routes> tab_routes = Make<Routes>();
  std::shared_ptr<Checkout> tab_checkout = Make<Checkout>();

  Component tab_about =
      Renderer([] { return hbox(text(L"About") | center) | flex | border; });

  int tab_selected = 0;
  std::vector<std::wstring> tab_entries = {
      L"Home",
      L"Routes",
      L"Checkout",
      L"About",
  };
  auto tab_selection = Toggle(&tab_entries, &tab_selected);

  Component tab_content = Container::Tab(
      {
          tab_home,
          tab_routes,
          tab_checkout,
          tab_about,
      },
      &tab_selected);

  auto component = Container::Vertical({
      tab_selection,
      tab_content,
  });


  component = Renderer(component, [&] {
    return vbox({
        text(L"Daniel's Bus System") | bold | hcenter,
        tab_selection->Render() | hcenter,
        tab_content->Render() | flex,
    });
  });

  auto shared_data = std::make_shared<RoutesData>();
  tab_routes->routesDataRef = shared_data;
  tab_checkout->routesDataRef = shared_data;

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(component);

  return 0;
}
