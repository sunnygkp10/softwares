<?hh
namespace APIs\Domain\Configurations\Functions\Languages;

interface LanguageDefinitionAdapter {
    public function convertMapToLanguageDefinition(Map<string, string> $map): LanguageDefinition;
}
