<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Variables\Variable;

final class ConcreteVariable implements Variable {

    private $name;
    private $keynames;
    public function __construct(string $name, string $delimiter) {

        if (empty($name)) {
            throw new \Exception('The variable name cannot be empty.');
        }

        if (empty($delimiter)) {
            throw new \Exception('The delimiter cannot be empty.');
        }

        if (strpos($name, $delimiter) === false) {
            throw new \Exception('There must be at least 1 instance of the delimiter ('.$delimiter.') inside the variable ('.$name.').  None found.');
        }

        $this->name = $name;
        $this->keynames = $this->generateKeynames($name, $delimiter);

    }

    private function generateKeynames(string $name, string $delimiter): Vector<string> {

        $output = Vector<string> {};
        $keynames = explode($delimiter, $name);
        foreach($keynames as $oneKeyname) {

            if (empty($oneKeyname)) {
                throw new \Exception('There is an empty keyname inside the variable.  This is probably due because there is a delimiter at the end of the variable and/or multiple occurence of the delimiter consecutively inside the variable.  The variable is: "'.$name.'".  The delimiter is: "'.$delimiter.'"');
            }

            $output[] = $oneKeyname;
        }

        return $output;
    }

    public function getName(): string {
        return $this->name;
    }

    public function getKeynames(): Vector<string> {
        return $this->keynames;
    }

    public function fetch(Map<string, mixed> $data): mixed {

        $recursiveFetch = function(Map<string, mixed> $data, Vector<string> $keynames) use(&$recursiveFetch) {

            $keyname = $keynames[0];
            if (!isset($data[$keyname])) {
                throw new \Exception('The variable ('.$this->name.') could not be found in the given data.  The problem occured at the keyname: '.$keyname);
            }

            $keynames->removeKey(0);
            $keynames = $keynames->values();

            if (empty($keynames)) {
                return $data[$keyname];
            }

            return $recursiveFetch($data[$keyname], $keynames);

        };

        $keynames = clone $this->keynames;
        return $recursiveFetch($data, $keynames);

    }

    public function compare(Variable $variable): boolean {
        return ($this->name == $variable->getName());
    }
}
