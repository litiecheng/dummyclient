#pragma once

#include <vector>

#include "core/character.hpp"
#include "model/model.hpp"


namespace Model {

class CharactersListModel : public Model {
public:
    using CharactersList =
        std::vector<std::shared_ptr<Dummy::Core::Character>>;
    CharactersListModel();
    virtual ~CharactersListModel();
    void addCharacter(std::shared_ptr<Dummy::Core::Character>);
    const CharactersList& characters() const {
        return m_characters;
    }

    virtual void
    visit(std::shared_ptr<Screen::SelectCharacterScreen>) override;
    virtual void
    visit(std::shared_ptr<Screen::CreateCharacterScreen>) override;
private:
    CharactersList m_characters;
    std::shared_ptr<Dummy::Core::Character> m_selectedCharacter;
};

} // namespace model
