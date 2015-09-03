<?hh
namespace APIs\Domain\Configurations\Variables;

interface VariableAdapter {
    public function convertStringToVariable(string $variable): Variable;
}
