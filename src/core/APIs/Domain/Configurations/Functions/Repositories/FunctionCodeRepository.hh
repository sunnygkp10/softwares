<?hh
namespace APIs\Domain\Configurations\Functions\Repositories;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\RawConfiguration;

interface FunctionCodeRepository {
    public function retrieveMultiple(RawConfiguration $configuration): Map<string, FunctionCode>;
}
