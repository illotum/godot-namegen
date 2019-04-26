#include "markovnames.hpp"
#include <Godot.hpp>
#include <Array.hpp>
#include <Reference.hpp>
#include <codecvt>
#include <locale>
#include <iostream>
#include <string>


class NameGenerator : public godot::Reference {
  GODOT_CLASS(NameGenerator, Reference);
public:
  NameGenerator() { }

  void _init() { }

  static void _register_methods() {
    godot::register_method("new_word", &NameGenerator::new_word);
    godot::register_method("new_words", &NameGenerator::new_words);
    godot::register_method("train", &NameGenerator::train);
    godot::register_method("is_trained", &NameGenerator::is_trained);
  }

  void train(const godot::PoolStringArray arr)  {
    std::vector<std::wstring> data;
    /* std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter; */

    { // locked scope
      auto read = arr.read();
      for (int i = 0; i < arr.size(); i++) {
        /* std::wstring s = converter.from_bytes(read[i].utf8().get_data()); */
        std::wstring s(read[i].unicode_str());
        data.push_back(s);
      }
    } // locked scope
    m_generator.train(data);
  }

  bool is_trained() const {
    return m_generator.isTrained();
  }

  godot::String new_word(int min , int max) const {
    return godot::String(m_generator.newWord(min, max).c_str());
  }

  godot::PoolStringArray new_words(int n, int min, int max) const {
    godot::PoolStringArray arr;
    auto words = m_generator.newWords(n, min, max);
    for (const std::wstring &s : words) {
      arr.push_back(godot::String(s.c_str()));
    }
    return arr;
  }

private:
  WordGenerator m_generator;
};

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
	godot::Godot::nativescript_init(handle);

	godot::register_class<NameGenerator>();
}
