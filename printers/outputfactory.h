#ifndef OUTPUTFACTORY_H
#define OUTPUTFACTORY_H

#include <QVector>
#include <QString>
#include <memory>
#include "nativeoutput.h"

namespace Printers {

    class OutputFactory
    {
    private:
        template<class TDerived>
        static BaseOutputPtr creator()
        {
            return QSharedPointer<TDerived>::create();
        }

        // Instanciation maps
        using Creator = BaseOutputPtr(); // Function type definition
        using CreatorPair = QPair<QString, Creator*>;
        using CreatorVector = QVector<CreatorPair>;
        CreatorVector creatorFunctions;

        static std::shared_ptr<OutputFactory> sInstance;
        OutputFactory() {}

    public:
        static std::shared_ptr<OutputFactory> instance()
        {
            return sInstance;
        }

        template<class TDerived>
        void registerOutput(QString label)
        {
            static_assert (std::is_base_of<NativeOutput, TDerived>::value, "OutputFactory::registerOutput doesn't accept this type because doesn't derive from base class");
            creatorFunctions.append(CreatorPair(label, &creator<TDerived>));
        }

        BaseOutputPtr createOutput(QString label) const
        {
            for(const auto& it : creatorFunctions)
            {
                if (it.first == label)
                {
                    return it.second();
                }
            }
            throw new std::invalid_argument("Unknown output label given.");
        }

        int numRegisteredOutputs() const
        {
            return creatorFunctions.size();
        }

        const QVector<QString> getOutputNames() const
        {
            QVector<QString> names;

            for(const auto& it : creatorFunctions)
            {
                names.append(it.first);
            }
            return names;
        }

    };
}
#endif // OUTPUTFACTORY_H
