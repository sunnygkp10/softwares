<?hh
namespace APIs\Domain\Configurations\Functions;

interface FunctionCodeAdapter {
    public function convertDataToFunctionCodes(Map<string, mixed> $data): Map<string, FunctionCode>;
    public function convertDataToFunctionCode(Map<string, mixed> $data): FunctionCode;
}
