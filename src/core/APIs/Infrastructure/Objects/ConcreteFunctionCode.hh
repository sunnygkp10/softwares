<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Functions\Languages\LanguageDefinition;

final class ConcreteFunctionCode implements FunctionCode {
    private $name;
    private $language;
    private $code;
    public function __construct(string $name, LanguageDefinition $language, string $code) {

        if (empty($name)) {
            throw new \Exception('The function code cannot have an empty name.');
        }

        if (empty($code)) {
            throw new \Exception('The function code cannot have empty code.');
        }

        $this->name = $name;
        $this->language = $language;
        $this->code = $code;
    }

    public function getName(): string {
        return $this->name;
    }

    public function getLanguage(): LanguageDefinition {
        return $this->language;
    }

    public function getCode(): string {
        return $this->code;
    }

}
