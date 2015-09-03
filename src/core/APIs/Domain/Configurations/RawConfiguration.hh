<?hh
namespace APIs\Domain\Configurations;

interface RawConfiguration {
    public function getOriginal();
    public function has(string $variable);
    public function get(string $variable);
}
