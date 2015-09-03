<?hh
namespace APIs\Domain\Configurations\Functions\Languages;
use APIs\Domain\Https\URLs\URL;

interface LanguageDefinition {
    public function getName(): string;
    public function getHost(): URL;
    public function getPort(): integer;
}
