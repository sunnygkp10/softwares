<?hh
namespace APIs\Domain\Configurations\Variables;

interface Variable {
    public function getName(): string;
    public function getKeynames(): Vector<string>;
    public function fetch(Map<string, mixed> $data): mixed;
    public function compare(Variable $variable): boolean;
}
