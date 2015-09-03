<?hh
namespace APIs\Domain\Configurations\Functions;
use APIs\Domain\Configurations\Functions\Languages\LanguageDefinition;

interface FunctionCode {
    public function getName(): string;
    public function getLanguage(): LanguageDefinition;
    public function getCode(): string;
}
