<?hh
namespace APIs\Domain\Configurations\Validators;
use APIs\Domain\Configurations\RawConfiguration;

interface ValidatorAdapter {
    public function convertRawConfigurationToValidators(RawConfiguration $configuration): Map<string, Validator>;
}
