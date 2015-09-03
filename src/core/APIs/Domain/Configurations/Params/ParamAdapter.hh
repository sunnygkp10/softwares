<?hh
namespace APIs\Domain\Configurations\Params;

interface ParamAdapter {
    public function convertDataToParams(Map<string, mixed> $data): Map<string, ?Param>;
    public function convertDataToParam(Map<string, mixed> $data): Param;
    public function convertStringToParam(string $param): Param;
}
