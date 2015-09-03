<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Functions\Languages\LanguageDefinition;
use APIs\Domain\Https\URLs\URL;

final class ConcreteLanguageDefinition implements LanguageDefinition {
    private $name;
    private $host;
    private $port;
    public function __construct(string $name, URL $host, integer $port) {

        if (empty($name)) {
            throw new \Exception('The language definition cannot have an empty name.');
        }

        $this->name = $name;
        $this->host = $host;
        $this->port = $port;

    }

    public function getName(): string {
        return $this->name;
    }

    public function getHost(): URL {
        return $this->host;
    }

    public function getPort(): integer {
        return $this->port;
    }

}
