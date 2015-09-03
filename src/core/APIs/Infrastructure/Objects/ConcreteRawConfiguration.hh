<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\RawConfiguration;

final class ConcreteRawConfiguration implements RawConfiguration {
    private $original;
    private $configuration;
    private $variableDelimiter;
    public function __construct(string $original, Map<string, mixed> $configuration, string $variableDelimiter) {
        $this->original = $original;
        $this->configuration = $configuration;
        $this->variableDelimiter = $variableDelimiter;
    }

    public function getOriginal() {
        return $this->original;
    }

    public function has(string $variable) {

        try {

            $this->get($variable);
            return true;

        } catch (\Exception $exception) {

        }

        return false;

    }

    public function get(string $variable) {

        $getByKeyname = function(string $keyname, Map<string, mixed> $configuration) {

            if (isset($configuration[$keyname])) {
                return $configuration[$keyname];
            }

            return null;

        };

        $keynames = explode($this->variableDelimiter, $variable);
        $configuration = $this->configuration;
        foreach($keynames as $onKeyname) {

            if (!isset($configuration[$onKeyname])) {
                throw new \Exception('The value of the variable ('.$variable.') could not be found.  The specific keyname ('.$onKeyname.') could not be retrieved from the configurations because its parent did not point to an array.', 500);
            }

            $configuration = $getByKeyname($onKeyname, $configuration);
            if (is_null($configuration)) {
                throw new \Exception('The value of the variable ('.$variable.') could not be found.  The specific keyname ('.$onKeyname.') could not be retrieved from the configurations.  Its parent pointed to a null element.', 500);
            }
        }

        return $configuration;
    }

}
